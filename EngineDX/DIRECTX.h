#ifndef DIRECTX
#define DIRECTX

#include "HR_debug.h"

#include <d3d11.h>
#include <dxgi.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const bool FULL_SCREEN = false;
const bool VSYNC = true;

class DirectXClass
{
public:
    DirectXClass();
    ~DirectXClass();

    bool Init(HWND);
    void Shutdown();

    void BeginScene(float, float, float, float);
    void EndScene();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();

private:
    bool InitDeviceAndSwapChain(HWND, UINT, UINT);
    bool InitRenderTargets(UINT, UINT);
    bool InitRasterState(UINT, UINT);

private:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    ID3D11RenderTargetView* m_pRenderTargetView;
    IDXGISwapChain* m_pSwapChain;

    ID3D11RasterizerState* m_pRasterState;
    ID3D11Texture2D*  m_pDepthTexture;
    ID3D11DepthStencilView* m_pDepthStencilView;
};

#endif // !DIRECTX
