#pragma once

class CLight;

class CShadowShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
	};

	struct LightBufferType2
	{
		XMFLOAT3 lightPosition;
		float padding;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_inputLayout;
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer*		m_matrixBuffer;
	ID3D11Buffer*		m_lightBuffer;
	ID3D11Buffer*		m_lightBuffer2;

public:
	void Init(ID3D11Device* _device, HWND _hwnd);

	void Render(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light, UINT _indexCount);

	void SetShaderParameters(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light);
	void RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount);

	void OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName);

public:
	CShadowShader();
	~CShadowShader();
};

