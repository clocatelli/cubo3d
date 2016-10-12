#include "DIRECTX.h"

DirectXClass::DirectXClass()
{
    m_pDevice = nullptr;
    m_pContext = nullptr;
    m_pRenderTargetView = nullptr;
    m_pSwapChain = nullptr;
}

DirectXClass::~DirectXClass()
{
}

bool DirectXClass::Init(HWND hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT uWidth = rc.right - rc.left;
    UINT uHeight = rc.bottom - rc.top;


    if (!InitDeviceAndSwapChain(hWnd, uWidth, uHeight))
        return false;

    if (!InitRenderTargets(uWidth, uHeight))
        return false;

    if (!InitRasterState(uWidth, uHeight))
        return false;

    return true;
}

bool DirectXClass::InitDeviceAndSwapChain(HWND hWnd, UINT uWidth, UINT uHeight)
{

    IDXGIFactory* pFactory = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
    if (FAILED(hr))
        return false;

    IDXGIAdapter* pAdapter = nullptr;
    hr = pFactory->EnumAdapters(0, &pAdapter);
    if (FAILED(hr))
        return false;

    IDXGIOutput* pOutput = nullptr;
    hr = pAdapter->EnumOutputs(0, &pOutput);
    if (FAILED(hr))
        return false;

    UINT uNumModes = 0;
    hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &uNumModes, nullptr);
    if (FAILED(hr))
        return false;

    DXGI_MODE_DESC* pModeList = new DXGI_MODE_DESC[uNumModes];
    if (!pModeList)
        return false;

    hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &uNumModes, pModeList);
    if (FAILED(hr))
        return false;

    UINT uDenominator = 0, uNumerator = 0;
    for (UINT i = 0; i < uNumModes; ++i)
    {
        if (pModeList[i].Width == uWidth)
        {
            if (pModeList[i].Height == uHeight)
            {
                uNumerator = pModeList[i].RefreshRate.Numerator;
                uDenominator = pModeList[i].RefreshRate.Denominator;
            }
        }
    }
    delete[] pModeList;
    pModeList = nullptr;

    pOutput->Release();
    pAdapter->Release();
    pFactory->Release();
    pOutput = nullptr;
    pAdapter = nullptr;
    pFactory = nullptr;

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT uNumFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT uNumDrviers = ARRAYSIZE(driverTypes);

    DXGI_SWAP_CHAIN_DESC swapDesc;
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = uWidth;
    swapDesc.BufferDesc.Height = uHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    if (VSYNC)
    {
        swapDesc.BufferDesc.RefreshRate.Numerator = uNumerator;
        swapDesc.BufferDesc.RefreshRate.Denominator = uDenominator;
    }
    else
    {
        swapDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.Flags = 0;
    swapDesc.OutputWindow = hWnd;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if (FULL_SCREEN)
    {
        swapDesc.Windowed = false;
    }
    else
    {
        swapDesc.Windowed = true;
    }

    for (UINT driver = 0; driver < uNumDrviers; driver++)
    {
        hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[driver], 0, 0, featureLevels, uNumFeatureLevels,
            D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, 0, &m_pContext);

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return false;

    return true;
}

bool DirectXClass::InitRenderTargets(UINT uWidth, UINT uHeight)
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr))
        return false;

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    if (FAILED(hr))
        return false;

    // DEPTH BUFFER
    D3D11_TEXTURE2D_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthDesc.ArraySize = 1;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.Height = uHeight;
    depthDesc.Width = uWidth;
    depthDesc.MipLevels = 1;
    depthDesc.MiscFlags = 0;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = m_pDevice->CreateTexture2D(&depthDesc, nullptr, &m_pDepthTexture);
    if (FAILED(hr))
        return false;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = depthDesc.Format;
    descDSV.Texture2D.MipSlice = 0;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = m_pDevice->CreateDepthStencilView(m_pDepthTexture, &descDSV, &m_pDepthStencilView);
    if (FAILED(hr))
        return false;

    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    return true;
}

bool DirectXClass::InitRasterState(UINT uWidth, UINT uHeight)
{
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = true;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    HRESULT hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
    if (FAILED(hr))
        return false;

    m_pContext->RSSetState(m_pRasterState);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(uWidth);
    vp.Height = static_cast<float>(uHeight);
    vp.MaxDepth = 1.0f;
    vp.MinDepth = 0.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;

    m_pContext->RSSetViewports(1, &vp);

    return true;
}

void DirectXClass::Shutdown()
{
    if (m_pRenderTargetView)
    {
        m_pRenderTargetView->Release();
        m_pRenderTargetView = nullptr;
    }
    if (m_pSwapChain)
    {
        m_pSwapChain->Release();
        m_pSwapChain = nullptr;
    }
    if (m_pContext)
    {
        m_pContext->Release();
        m_pContext = nullptr;
    }
    if (m_pDevice)
    {
        m_pDevice->Release();
        m_pDevice = nullptr;
    }
}

void DirectXClass::BeginScene(float r, float g, float b, float a)
{
    float fClearColor[4];
    fClearColor[0] = r;
    fClearColor[1] = g;
    fClearColor[2] = b;
    fClearColor[3] = a;

    m_pContext->ClearRenderTargetView(m_pRenderTargetView, fClearColor);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void DirectXClass::EndScene()
{
    if (VSYNC)
    {
        m_pSwapChain->Present(1, 0);
    }
    else
    {
        m_pSwapChain->Present(0, 0);
    }
}

ID3D11Device* DirectXClass::GetDevice()
{
    return m_pDevice;
}

ID3D11DeviceContext* DirectXClass::GetContext()
{
    return m_pContext;
}