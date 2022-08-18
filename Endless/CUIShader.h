#pragma once

class CUIShader
{
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_inputLayout;
	ID3D11Buffer*		m_constantBuffer;
	ID3D11SamplerState* m_samplerState;
	
private:
	void SetShaderParameters(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture);

	void RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount);

	void OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName);

public:
	void Init(ID3D11Device* _device, HWND _hwnd);

	void Render(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture, UINT _indexCount);

public:
	CUIShader();
	~CUIShader();
};

