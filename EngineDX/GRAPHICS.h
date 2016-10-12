#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Windows.h>
#include "DIRECTX.h"
#include "MODEL.h"
#include "SHADER.h"

class Graphics
{
public:
    Graphics();
    ~Graphics();

    bool Init(HWND);
    void Shutdown();
    bool Frame(float dt);

private:

    bool Render();

private:
    DirectXClass* m_pDirectX;
    Model* m_pModel;
    Shader* m_pShader;

    MatricesCB* m_pMatrixCB;
};




#endif // !GRAPHICS_H
