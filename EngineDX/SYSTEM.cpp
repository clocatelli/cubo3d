#include "SYSTEM.h"


System::System()
{
    m_pInput = nullptr;
    m_pGraphics = nullptr;
    m_pTimer = nullptr;
}

System::~System()
{

}

bool System::Init()
{
    if (!InitWindow())
        return false;

    m_pInput = new Input();
    if (!m_pInput)
        return false;

    m_pInput->Init();

    m_pGraphics = new Graphics();
    if (!m_pGraphics)
        return false;

    bool bError = m_pGraphics->Init(m_hWnd);
    if (!bError)
        return false;

    // TIMER
    m_pTimer = new Timer();
    if (!m_pTimer)
        return false;

    if (!m_pTimer->Init())
        return false;

    return true;
}

void System::Shutdown()
{
    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }

    if (m_pInput)
    {
        delete m_pInput;
        m_pInput = nullptr;
    }

    if (m_pGraphics)
    {
        m_pGraphics->Shutdown();
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }

    ShutdownWindow();
}

void System::MainLoop()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    bool bDone = false, bError;

    while (!bDone)
    {
        if (msg.message == WM_QUIT)
        {
            bDone = true;
            continue;
        }
        
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            bError = Frame();
            if (!bError)
                bDone = false;
        }
    }

}

LRESULT CALLBACK System::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        m_pInput->KeyDown((unsigned int)wParam);
        break;
    case WM_KEYUP:
        m_pInput->KeyUp((unsigned int)wParam);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    return 0;
}

bool System::Frame()
{
    if (m_pInput->IsKeyDown(VK_ESCAPE))
        return false;

    m_pTimer->Frame();

    bool bError = m_pGraphics->Frame(m_pTimer->GetTime());
    if (!bError)
        return false;
    
    return true;
}

bool System::InitWindow()
{
    g_pAppHandle = this;
    m_hInstance = GetModuleHandle(0);
    m_sAppName = L"DirectX 11 Engine";

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hInstance = m_hInstance;
    wc.lpfnWndProc = MainWindowProc;
    wc.lpszClassName = m_sAppName;
    wc.lpszMenuName = 0;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wc))
        return false;

    int posX;
    int posY;
    DWORD style;
    if (FULL_SCREEN)
    {
        style = WS_POPUP;
        posX = 0;
        posY = 0;
        m_iWidth = GetSystemMetrics(SM_CXSCREEN);

        m_iHeight = GetSystemMetrics(SM_CYSCREEN);

    }
    else
    {
        style = WS_OVERLAPPEDWINDOW;
        RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        AdjustWindowRect(&rc, style, false);
        m_iWidth = rc.right - rc.left;
        m_iHeight = rc.bottom - rc.top;
        posX = (GetSystemMetrics(SM_CXSCREEN) - m_iWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - m_iHeight) / 2;
    }

    m_hWnd = CreateWindow(m_sAppName, m_sAppName, style, posX, posY, m_iWidth, m_iHeight,
                          0, 0, m_hInstance, 0);

    if (!m_hWnd)
        return false;

    ShowWindow(m_hWnd, SW_SHOW);

    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);

    return true;
}

void System::ShutdownWindow()
{
    DestroyWindow(m_hWnd);
    m_hWnd = 0;

    UnregisterClass(m_sAppName, m_hInstance);
    m_hInstance = 0;

    g_pAppHandle = nullptr;
}

static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return g_pAppHandle->MessageHandler(hWnd, uMsg, wParam, lParam);
        break;
    }

    return 0;
}