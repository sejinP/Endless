#pragma once

class CD3DApp
{
private:
	D3D_DRIVER_TYPE			m_driverType;
	D3D_FEATURE_LEVEL		m_featureLevel;

	ID3D11Device*			m_d3dDevice;
	ID3D11DeviceContext*	m_immediateContext;

	IDXGISwapChain*			m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;

	ID3D11Texture2D*		m_depthStencil;
	ID3D11DepthStencilView* m_depthStencilView;

	D3D11_VIEWPORT			m_viewport;

	ID3D11DepthStencilState* m_depthStencilState;			// z버퍼 온
	ID3D11DepthStencilState* m_depthDisabledStencilState;	// z버퍼 오프
	ID3D11RasterizerState*	 m_rasterState;

public:
	ID3D11Device* GetDevice() { return m_d3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_immediateContext; }

	void SetFullScreen(bool _b) { m_swapChain->SetFullscreenState(_b, nullptr); }

	void SetBackRenderTarget();

	void TurnZBufferOn();
	void TurnZBufferOff();

public:
	HRESULT InitDevice(HWND _hwnd, int _screenWidth, int _screenHeight, bool _fullScreen);
	void ClearView();
	void ChainSwap();

public:
	CD3DApp();
	virtual ~CD3DApp();
};

