#include "global.h"
#include "CColliderShader.h"

#include "CCentral.h"

CColliderShader::CColliderShader()
	: m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_inputLayout(nullptr)
	, m_transformBuffer(nullptr)

{
	
}

CColliderShader::~CColliderShader()
{
	if (m_vertexShader)
		m_vertexShader->Release();
	if (m_pixelShader)
		m_pixelShader->Release();
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_transformBuffer)
		m_transformBuffer->Release();
}

void CColliderShader::Init(ID3D11Device* _device, HWND _hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// 절대 경로
	wstring absolutePath = CCentral::Instance().GetAbsolutePath();
	wstring vsPath = absolutePath + L"shader\\collider_vs.hlsl";
	wstring psPath = absolutePath + L"shader\\collider_ps.hlsl";
	const wchar_t* vsFileName = vsPath.c_str();
	const wchar_t* psFileName = psPath.c_str();

	// 버텍스 셰이더 코드 컴파일
	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(vsFileName, nullptr, nullptr, "main", "vs_5_0",
		dwShaderFlags, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		// 셰이더 컴파일 실패시 오류 메시지 출력
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"collider_vs");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"collider_vs", L"Missing Shader File", MB_OK);
		}
	}

	// 픽셀 셰이더 컴파일.
	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(psFileName, nullptr, nullptr, "main", "ps_5_0",
		dwShaderFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// 셰이더 컴파일 실패시 오류메시지를 출력
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"collider_ps");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"collider_ps", L"Missing Shader File", MB_OK);
		}
	}

	// 버퍼로부터 정점 셰이더를 생성.
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		NULL, &m_vertexShader);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 버퍼에서 픽셀 셰이더 생성
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		NULL, &m_pixelShader);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 정점 입력 레이아웃 구조체 설정
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃의 요소 수 가져오기
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 정점 입력 레이아웃 만들기
	result = _device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_inputLayout);

	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 블롭 해제
	vertexShaderBuffer->Release();

	pixelShaderBuffer->Release();

	// 정점 셰이더의 행렬 상수 버퍼 구조체
	D3D11_BUFFER_DESC transformBufferDesc;
	transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformBufferDesc.ByteWidth = sizeof(TransformBufferType);
	transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformBufferDesc.MiscFlags = 0;
	transformBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 생성
	result = _device->CreateBuffer(&transformBufferDesc, NULL, &m_transformBuffer);
	if (FAILED(result))
	{
		assert(nullptr);
	}
}

void CColliderShader::OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName)
{
	// 에러 메시지를 출력창에 표시
	OutputDebugStringA(reinterpret_cast<const char*>(_errorMessage->GetBufferPointer()));

	// 에러 메세지를 반환
	_errorMessage->Release();

	// 컴파일 에러가 있음을 팝업 메세지로 알림
	MessageBox(_hwnd, L"Error compiling shader.", _shaderFileName, MB_OK);
}

void CColliderShader::Render(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix, UINT _indexCount)
{
	SetShaderParameters(_deviceContext, _transformMatrix);

	RenderShader(_deviceContext, _indexCount);
}

void CColliderShader::SetShaderParameters(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix)
{
	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 함.
	_transformMatrix.worldMatrix = XMMatrixTranspose(_transformMatrix.worldMatrix);
	_transformMatrix.viewMatrix = XMMatrixTranspose(_transformMatrix.viewMatrix);
	_transformMatrix.projectionMatrix = XMMatrixTranspose(_transformMatrix.projectionMatrix);

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// 행렬 변환 상수 버퍼 설정
	// 상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_deviceContext->Map(m_transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	TransformBufferType* dataPtr = (TransformBufferType*)mappedResource.pData;

	dataPtr->worldMatrix = _transformMatrix.worldMatrix;
	dataPtr->viewMatrix = _transformMatrix.viewMatrix;
	dataPtr->projectionMatrix = _transformMatrix.projectionMatrix;

	_deviceContext->Unmap(m_transformBuffer, 0);

	unsigned bufferNumber = 0;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_transformBuffer);
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
}

void CColliderShader::RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount)
{
	// 정점 입력 레이아웃, 버텍스 셰이더, 픽셀 셰이더 설정
	_deviceContext->IASetInputLayout(m_inputLayout);
	_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 삼각형을 그리기
	_deviceContext->DrawIndexed(_indexCount, 0, 0);
}