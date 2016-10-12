#include "GRAPHICS.h"

Graphics::Graphics()
{
    m_pDirectX = nullptr;
    m_pModel = nullptr;
    m_pShader = nullptr;

    m_pMatrixCB = nullptr;
}

Graphics::~Graphics()
{
}

bool Graphics::Init(HWND hWnd)
{
    m_pDirectX = new DirectXClass();
    if (!m_pDirectX)
        return false;
    
    if (!m_pDirectX->Init(hWnd))
        return false;

    // MODEL
    m_pModel = new Model();
    if (!m_pModel)
        return false;

    if (!m_pModel->Init(m_pDirectX->GetDevice(), L"stone.dds"))
        return false;

    // SHADER
    m_pShader = new Shader();
    if (!m_pShader)
        return false;

    if (!m_pShader->Init(m_pDirectX->GetDevice(), L"VertexShader.hlsl", L"PixelShader.hlsl"))
        return false;

    // MATRICES
    m_pMatrixCB = new MatricesCB();
    if (!m_pMatrixCB)
        return false;

    m_pMatrixCB->mWorld = DirectX::XMMatrixIdentity();

    DirectX::XMVECTOR vEye = DirectX::XMVectorSet(0.0f, 0.0f, 3.5f, 0.0f);
    DirectX::XMVECTOR vAt  = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR vUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtRH(vEye, vAt, vUp);
    m_pMatrixCB->mView = DirectX::XMMatrixTranspose(mView);

    float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    DirectX::XMMATRIX mProj = DirectX::XMMatrixPerspectiveFovRH(DirectX::XM_PIDIV4, aspect, 0.01f, 100.0f);
    m_pMatrixCB->mProj = DirectX::XMMatrixTranspose(mProj);

    return true;
}

void Graphics::Shutdown()
{
    if (m_pShader)
    {
        m_pShader->Shutdown();
        delete m_pShader;
        m_pShader = nullptr;
    }

    if (m_pModel)
    {
        m_pModel->Shutdown();
        delete m_pModel;
        m_pModel = nullptr;
    }

    if (m_pDirectX)
    {
        m_pDirectX->Shutdown();
        delete m_pDirectX;
        m_pDirectX = nullptr;
    }
}

bool Graphics::Frame(float dt)
{
    DirectX::XMMATRIX mWorld = DirectX::XMMatrixRotationRollPitchYaw(dt, dt, dt);
    m_pMatrixCB->mWorld = DirectX::XMMatrixTranspose(mWorld);

    if (!Render())
        return false;

    return true;
}

bool Graphics::Render()
{
    m_pDirectX->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

    m_pShader->SetShaderParameters(m_pDirectX->GetContext(), m_pModel->GetTexture(), m_pMatrixCB);
    m_pModel->Render(m_pDirectX->GetContext());
    m_pShader->Render(m_pDirectX->GetContext(), m_pModel->GetIndexCount());

    m_pDirectX->EndScene();

    return true;
}

