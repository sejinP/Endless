#include "global.h"
#include "CUIShader.h"

#include "CCentral.h"

CUIShader::CUIShader()
	: m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_inputLayout(nullptr)
	, m_constantBuffer(nullptr)
	, m_samplerState(nullptr)
{
	
}

CUIShader::~CUIShader()
{
	if (m_vertexShader)
		m_vertexShader->Release();
	if (m_pixelShader)
		m_pixelShader->Release();
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_constantBuffer)
		m_constantBuffer->Release();
	if (m_samplerState)
		m_samplerState->Release();
}

void CUIShader::Init(ID3D11Device* _device, HWND _hwnd)
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
	wstring vsPath = absolutePath + L"shader\\ui_vs.hlsl";
	wstring psPath = absolutePath + L"shader\\ui_ps.hlsl";
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
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"ui_vs");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"ui_vs", L"Missing Shader File", MB_OK);
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
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"ui_ps");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"ui_ps", L"Missing Shader File", MB_OK);
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
	// 포지션과 텍스쳐좌표만 사용(UI버퍼의 버텍스 타입과 같아야 함)
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
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
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 생성
	result = _device->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 텍스처 샘플러 상태 구조체
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 샘플러 상태 생성
	result = _device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		assert(nullptr);
	}
}

void CUIShader::OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName)
{
	// 에러 메시지를 출력창에 표시
	OutputDebugStringA(reinterpret_cast<const char*>(_errorMessage->GetBufferPointer()));

	// 에러 메세지를 반환
	_errorMessage->Release();

	// 컴파일 에러가 있음을 팝업 메세지로 알림
	MessageBox(_hwnd, L"Error compiling shader.", _shaderFileName, MB_OK);
}

void CUIShader::Render(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix,
	ID3D11ShaderResourceView* _texture, UINT _indexCount)
{
	// 상수 버퍼 설정
	SetShaderParameters(_deviceContext, _transformMatrix, _texture);

	// 셰이더 세팅 후 렌더
	RenderShader(_deviceContext, _indexCount);
}

void CUIShader::SetShaderParameters(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix, ID3D11ShaderResourceView* _texture)
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
	_deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->worldMatrix = _transformMatrix.worldMatrix;
	dataPtr->viewMatrix = _transformMatrix.viewMatrix;
	dataPtr->projectionMatrix = _transformMatrix.projectionMatrix;

	_deviceContext->Unmap(m_constantBuffer, 0);

	unsigned bufferNumber = 0;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_constantBuffer);
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------

	// 픽셀 셰이더에서 셰이더 텍스쳐 리소스를 설정.
	_deviceContext->PSSetShaderResources(0, 1, &_texture);
}

void CUIShader::RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount)
{
	// 정점 입력 레이아웃, 버텍스 셰이더, 픽셀 셰이더 설정
	_deviceContext->IASetInputLayout(m_inputLayout);
	_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	// 샘플러 상태를 설정
	_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	// 삼각형을 그리기
	_deviceContext->DrawIndexed(_indexCount, 0, 0);
}