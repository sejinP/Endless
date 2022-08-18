#pragma once

class CDepthTexture
{
private:
	ID3D11Texture2D*			m_depthTexture;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11ShaderResourceView*	m_shaderResourceView;

	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilView*		m_depthStencilView;
	D3D11_VIEWPORT				m_viewport;

	// 4096 x 4096 의 깊이 텍스쳐
	const int	SHADOWMAP_WIDTH = 4096;
	const int	SHADOWMAP_HEIGHT = 4096;

public:
	void Init(ID3D11Device* _device);
	void SetRenderTarget(ID3D11DeviceContext* _deviceContext);

public:
	ID3D11ShaderResourceView* GetShaderResoucreView() { return m_shaderResourceView; }

public:
	CDepthTexture();
	~CDepthTexture();
};