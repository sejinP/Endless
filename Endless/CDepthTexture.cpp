#include "global.h"
#include "CDepthTexture.h"

CDepthTexture::CDepthTexture()
	: m_depthTexture(nullptr)
	, m_renderTargetView(nullptr)
	, m_shaderResourceView(nullptr)
	, m_depthStencilBuffer(nullptr)
	, m_depthStencilView(nullptr)
	, m_viewport{}
{
	
}

CDepthTexture::~CDepthTexture()
{
	if (m_depthTexture)
		m_depthTexture->Release();
	if (m_renderTargetView)
		m_renderTargetView->Release();
	if (m_shaderResourceView)
		m_shaderResourceView->Release();
	if (m_depthStencilBuffer)
		m_depthStencilBuffer->Release();
	if (m_depthStencilView)
		m_depthStencilView->Release();
}

void CDepthTexture::Init(ID3D11Device* _device)
{
	HRESULT result;

	// ·»´õ Å¸°Ù ÅØ½ºÃÄ ±¸Á¶Ã¼
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = SHADOWMAP_WIDTH;
	textureDesc.Height = SHADOWMAP_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	
	// ·»´õ Å¸°Ù ÅØ½ºÃÄ »ý¼º
	result = _device->CreateTexture2D(&textureDesc, nullptr, &m_depthTexture);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// ·»´õ Å¸°Ù ºä ±¸Á¶Ã¼
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// ·»´õ Å¸°Ù ºä »ý¼º
	result = _device->CreateRenderTargetView(m_depthTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// ¼ÎÀÌ´õ ¸®¼Ò½º ºä ±¸Á¶Ã¼
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ¼ÎÀÌ´õ ¸®¼Ò½º ºä »ý¼º
	result = _device->CreateShaderResourceView(m_depthTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// ±íÀÌ ¹öÆÛÀÇ ±¸Á¶Ã¼
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = SHADOWMAP_WIDTH;
	depthBufferDesc.Height = SHADOWMAP_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// ±íÀÌ ¹öÆÛ ÅØ½ºÃ³ »ý¼º
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// ±íÀÌ ½ºÅÙ½Ç ºä ±¸Á¶Ã¼
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ±íÀÌ ½ºÅÙ½Ç ºä »ý¼º
	result = _device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		assert(nullptr);
	}

	// ºäÆ÷Æ® ¼³Á¤
	m_viewport.Width = (float)SHADOWMAP_WIDTH;
	m_viewport.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
}

void CDepthTexture::SetRenderTarget(ID3D11DeviceContext* _deviceContext)
{
	// ¸®¼Ò½º ºä ÃÊ±âÈ­
	ID3D11ShaderResourceView* const view[128] = { NULL };
	_deviceContext->PSSetShaderResources(0, 128, view);

	// ·»´õ Å¸°Ù ¼³Á¤
	_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	_deviceContext->RSSetViewports(1, &m_viewport);

	// ·»´õ Å¸°Ù ºä¿Í ½ºÅÙ½Ç ºä Å¬¸®¾î
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}