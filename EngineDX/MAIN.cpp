#include "SYSTEM.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    System* pGame = new System();
    if (!pGame)
        return 1;

    bool bError = pGame->Init();
    if (!bError)
        return 1;

    pGame->MainLoop();
    pGame->Shutdown();
    delete pGame;
    pGame = nullptr;
    
    return 0;
}