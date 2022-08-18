#include "global.h"
#include "CShadowShader.h"

#include "CLight.h"
#include "CCentral.h"

CShadowShader::CShadowShader()
	: m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_inputLayout(nullptr)
	, m_sampleStateWrap(nullptr)
	, m_sampleStateClamp(nullptr)
	, m_matrixBuffer(nullptr)
	, m_lightBuffer(nullptr)
	, m_lightBuffer2(nullptr)
{
	
}

CShadowShader::~CShadowShader()
{
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_pixelShader->Release();
	if (m_inputLayout) m_inputLayout->Release();
	if (m_sampleStateWrap) m_sampleStateWrap->Release();
	if (m_sampleStateClamp) m_sampleStateClamp->Release();
	if (m_matrixBuffer) m_matrixBuffer->Release();
	if (m_lightBuffer) m_lightBuffer->Release();
	if (m_lightBuffer2) m_lightBuffer2->Release();
}

void CShadowShader::Init(ID3D11Device* _device, HWND _hwnd)
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
	wstring vsPath = absolutePath + L"shader\\shadow_vs.hlsl";
	wstring psPath = absolutePath + L"shader\\shadow_ps.hlsl";
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
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"shadow_vs");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"shadow_vs", L"Missing Shader File", MB_OK);
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
			OutputShaderErrorMessage(errorMessage, _hwnd, (WCHAR*)L"shadow_ps");
		}
		// 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우
		else
		{
			MessageBox(_hwnd, (WCHAR*)L"shadow_ps", L"Missing Shader File", MB_OK);
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

	// 정점 입력 레이아웃 구조체를 설정
	// VertexType 구조와 일치해야함.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// 레이아웃의 요소 수를 가져오기
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 정점 입력 레이아웃 생성
	result = _device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_inputLayout);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 블롭 해제.
	vertexShaderBuffer->Release();

	pixelShaderBuffer->Release();

	// Wrap 텍스처 샘플러 상태 구조체를 생성 및 설정
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

	// Wrap 텍스처 샘플러 상태 생성
	result = _device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// Clamp 텍스처 샘플러 상태
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// Clamp 텍스처 샘플러 상태 생성
	result = _device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체 작성
	// BIND_CONSTANT_BUFFER -> 바이트 크기 16의 배수
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 생성(행렬 변환에 관한)
	result = _device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 라이트 버퍼 구조체
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// 라이트 상수 버퍼 생성
	result = _device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// 버텍스 셰이더의 라이트 버퍼
	D3D11_BUFFER_DESC lightBufferDesc2;
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// 버텍스 셰이더의 라이트 버퍼 생성
	result = _device->CreateBuffer(&lightBufferDesc2, NULL, &m_lightBuffer2);
	if (FAILED(result))
	{
		assert(nullptr);
	}
}

void CShadowShader::OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName)
{
	// 에러 메시지를 출력창에 표시
	OutputDebugStringA(reinterpret_cast<const char*>(_errorMessage->GetBufferPointer()));

	// 에러 메세지를 반환
	_errorMessage->Release();

	// 컴파일 에러가 있음을 팝업 메세지로 알림
	MessageBox(_hwnd, L"Error compiling shader.", _shaderFileName, MB_OK);
}

void CShadowShader::Render(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
	ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light, UINT _indexCount)
{
	// 상수 버퍼 설정
	SetShaderParameters(_deviceContext, _transformMatrix, _texture, _depthMapTexture, _light);

	// 셰이더 세팅 후 렌더
	RenderShader(_deviceContext, _indexCount);
}

void CShadowShader::SetShaderParameters(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
	ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light)
{
	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 함.
	_transformMatrix.worldMatrix = XMMatrixTranspose(_transformMatrix.worldMatrix);
	_transformMatrix.viewMatrix = XMMatrixTranspose(_transformMatrix.viewMatrix);
	_transformMatrix.projectionMatrix = XMMatrixTranspose(_transformMatrix.projectionMatrix);
	_transformMatrix.lightViewMatrix = XMMatrixTranspose(_transformMatrix.lightViewMatrix);
	_transformMatrix.lightProjectionMatrix = XMMatrixTranspose(_transformMatrix.lightProjectionMatrix);

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// 행렬 변환 상수 버퍼 설정
	// 상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->worldMatrix = _transformMatrix.worldMatrix;
	dataPtr->viewMatrix = _transformMatrix.viewMatrix;
	dataPtr->projectionMatrix = _transformMatrix.projectionMatrix;
	dataPtr->lightViewMatrix = _transformMatrix.lightViewMatrix;
	dataPtr->lightProjectionMatrix = _transformMatrix.lightProjectionMatrix;

	_deviceContext->Unmap(m_matrixBuffer, 0);

	unsigned int bufferNumber = 0;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------

	// 픽셀 셰이더에서의 텍스쳐 리소스 설정
	_deviceContext->PSSetShaderResources(0, 1, &_texture);
	_deviceContext->PSSetShaderResources(1, 1, &_depthMapTexture);	// 깊이 맵 텍스쳐

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// 라이트 버퍼 설정
	_deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = _light->GetAmbientColor();
	dataPtr2->diffuseColor = _light->GetDiffuseColor();

	_deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// 두 번째 라이트 버퍼 설정
	_deviceContext->Map(m_lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	LightBufferType2* dataPtr3 = (LightBufferType2*)mappedResource.pData;

	dataPtr3->lightPosition = _light->GetLightPos();
	dataPtr3->padding = 0.0f;

	_deviceContext->Unmap(m_lightBuffer2, 0);

	// 버텍스 셰이더에서의 상수버퍼 위치
	bufferNumber = 1;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer2);
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
}

void CShadowShader::RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount)
{
	// 정점 입력 레이아웃, 버텍스 셰이더, 픽셀 셰이더 설정
	_deviceContext->IASetInputLayout(m_inputLayout);
	_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 샘플러 상태 설정
	_deviceContext->PSSetSamplers(0, 1, &m_sampleStateClamp);
	_deviceContext->PSSetSamplers(1, 1, &m_sampleStateWrap);

	// 삼각형을 그리기
	_deviceContext->DrawIndexed(_indexCount, 0, 0);
}