#ifndef SYSTEM_H
#define SYSTEM_H

#include "INPUT.h"
#include "GRAPHICS.h"
#include "TIMER.h"


class System
{
public:
    System();
    ~System();

    bool Init();
    void Shutdown();
    void MainLoop();
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool Frame();
    bool InitWindow();
    void ShutdownWindow();

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    int m_iWidth;
    int m_iHeight;
    LPCWSTR m_sAppName;

    Graphics* m_pGraphics;
    Input* m_pInput;
    Timer* m_pTimer;

};

static LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
static System* g_pAppHandle = 0;

#endif // !SYSTEM_H
