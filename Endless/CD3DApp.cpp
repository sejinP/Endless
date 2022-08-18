#include "global.h"
#include "CD3DApp.h"
#include <directxcolors.h>

CD3DApp::CD3DApp()
    : m_driverType()
    , m_featureLevel()
    , m_d3dDevice(nullptr)
	, m_immediateContext(nullptr)
	, m_swapChain(nullptr)
	, m_renderTargetView(nullptr)
    , m_depthStencil(nullptr)
    , m_depthStencilView(nullptr)
    , m_viewport{}
    , m_depthStencilState(nullptr)
    , m_depthDisabledStencilState(nullptr)
    , m_rasterState(nullptr)
{	  
    
}

CD3DApp::~CD3DApp()
{
    if (m_immediateContext) m_immediateContext->ClearState();

    if (m_rasterState) m_rasterState->Release();
    if (m_depthDisabledStencilState) m_depthDisabledStencilState->Release();
    if (m_depthStencilState) m_depthStencilState->Release();
    if (m_depthStencil) m_depthStencil->Release();
    if (m_depthStencilView) m_depthStencilView->Release();
    if (m_renderTargetView) m_renderTargetView->Release();
    if (m_swapChain) m_swapChain->Release();
    if (m_immediateContext) m_immediateContext->Release();
    if (m_d3dDevice) m_d3dDevice->Release();
}

// Direct3D 초기화
HRESULT CD3DApp::InitDevice(HWND _hwnd, int _screenWidth, int _screenHeight, bool _fullScreen)
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // 맞는 드라이버 타입의 디바이스 생성.
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &m_d3dDevice, &m_featureLevel, &m_immediateContext);

        if (hr == E_INVALIDARG)
        {
            hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &m_d3dDevice, &m_featureLevel, &m_immediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    // 디바이스로부터 팩토리 생성
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

	// 스왑체인 생성
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _screenWidth;
	sd.BufferDesc.Height = _screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

    if (_fullScreen)
    {
        // 풀스크린 모드
        sd.Windowed = FALSE;
    }
    else
    {
        // 창모드
        sd.Windowed = TRUE;
    }

    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
	hr = dxgiFactory->CreateSwapChain(m_d3dDevice, &sd, &m_swapChain);
    if (FAILED(hr))
        return hr;
    
    // 알트엔터 차단
    dxgiFactory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    // 렌더 타겟 뷰 생성
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_renderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    // 깊이 스텐실 텍스쳐 생성
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = _screenWidth;
    descDepth.Height = _screenHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_d3dDevice->CreateTexture2D(&descDepth, nullptr, &m_depthStencil);
    if (FAILED(hr))
        return hr;

    // 깊이 스텐실 뷰 생성
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = m_d3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
    if (FAILED(hr))
        return hr;

    m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // 스텐실 상태 구조체(z버퍼 온)
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 스텐실 상태 생성(z버퍼 온)
    hr = m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if (FAILED(hr))
        return hr;
    m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);

    // 스텐실 상태 구조체(z버퍼 오프)
    depthStencilDesc.DepthEnable = false;

    // 스텐실 상태 생성(z버퍼 오프)
    hr = m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);
    if (FAILED(hr))
        return hr;

    // 레스터라이저 구조체
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    hr = m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(hr))
        return hr;

    m_immediateContext->RSSetState(m_rasterState);

    // 뷰포트 셋업
    m_viewport.Width = (FLOAT)_screenWidth;
    m_viewport.Height = (FLOAT)_screenHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_immediateContext->RSSetViewports(1, &m_viewport);

    return S_OK;
}

void CD3DApp::ClearView()
{
    // 렌더 타겟 뷰와 깊이 스텐실 뷰 클리어
    m_immediateContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::GhostWhite);
    m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3DApp::ChainSwap()
{
    m_swapChain->Present(0, 0);
}

void CD3DApp::SetBackRenderTarget()
{
    // 렌더 타겟 뷰 원래대로 되돌려놓기
    m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // 뷰포트 되돌려놓기
    m_immediateContext->RSSetViewports(1, &m_viewport);

    // 렌더 타겟 뷰와 깊이 스텐실 뷰 클리어
    m_immediateContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::WhiteSmoke);
    m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3DApp::TurnZBufferOn()
{
    m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void CD3DApp::TurnZBufferOff()
{
    m_immediateContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}
