#include "MODEL.h"


Model::Model()
{
    m_pVertexBuffer = nullptr;
    m_pIndexBuffer = nullptr;
    m_pTexture = nullptr;
}

Model::~Model()
{
}

bool Model::Init(ID3D11Device* pDevice, WCHAR* sTextureName)
{
    if (!InitBuffers(pDevice))
        return false;

    if (!InitTextures(pDevice, sTextureName))
        return false;

    return true;
}

void Model::Shutdown()
{
    ShutdownBuffers();
    ShutdownTextures();
}

void Model::Render(ID3D11DeviceContext* pContext)
{
    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::InitBuffers(ID3D11Device* pDevice)
{
    Vertex vertices[] =
    {
        // FRONT
        { DirectX::XMFLOAT4(-0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 0
        { DirectX::XMFLOAT4(-0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 1
        { DirectX::XMFLOAT4( 0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 2
        { DirectX::XMFLOAT4( 0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 3
        
        // BACK                                                                               
        { DirectX::XMFLOAT4(-0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 4
        { DirectX::XMFLOAT4(-0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 5
        { DirectX::XMFLOAT4( 0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 6
        { DirectX::XMFLOAT4( 0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 7

        // TOP
        { DirectX::XMFLOAT4(-0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 1
        { DirectX::XMFLOAT4(-0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 5
        { DirectX::XMFLOAT4( 0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 6
        { DirectX::XMFLOAT4( 0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 2

        // BOTTOM
        { DirectX::XMFLOAT4(-0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 4        
        { DirectX::XMFLOAT4(-0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 0
        { DirectX::XMFLOAT4( 0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 3
        { DirectX::XMFLOAT4( 0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 7

        // RIGHT
        { DirectX::XMFLOAT4( 0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 3
        { DirectX::XMFLOAT4( 0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 2
        { DirectX::XMFLOAT4( 0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 6
        { DirectX::XMFLOAT4( 0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 7

        // LEFT
        { DirectX::XMFLOAT4(-0.6f, -0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 4
        { DirectX::XMFLOAT4(-0.6f,  0.6f, -0.6f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 5
        { DirectX::XMFLOAT4(-0.6f,  0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 1
        { DirectX::XMFLOAT4(-0.6f, -0.6f,  0.6f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 0
    };
    m_uVertexCount = 24;

    UINT indices[] =
    {
     // FRONT
        0, 2, 1,
        0, 3, 2,

     // BACK
        7, 5, 6,
        7, 4, 5,

     // TOP
        8, 10, 9,
        8, 11, 10,

     // BOTTOM
        12, 14, 13,
        12, 15, 14,
        
     // RIGHT
        16, 18, 17,
        16, 19, 18,

     // LEFT
        20, 22, 21,
        20, 23, 22
    };
    m_uIndexCount = 36;

    // VERTEX BUFFER
    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory(&vertexDesc, sizeof(D3D11_BUFFER_DESC));
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(vertices[0]) * m_uVertexCount;
    vertexDesc.CPUAccessFlags = 0;
    vertexDesc.MiscFlags = 0;
    vertexDesc.StructureByteStride = 0;
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    resourceData.pSysMem = vertices;

    HRESULT hr = pDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // INDEX BUFFER
    D3D11_BUFFER_DESC indexDesc;
    ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
    indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexDesc.ByteWidth = sizeof(indices[0]) * m_uIndexCount;
    indexDesc.CPUAccessFlags = 0;
    indexDesc.MiscFlags = 0;
    indexDesc.StructureByteStride = 0;
    indexDesc.Usage = D3D11_USAGE_DEFAULT;
    resourceData.pSysMem = indices;

    hr = pDevice->CreateBuffer(&indexDesc, &resourceData, &m_pIndexBuffer);
    if (FAILED(hr))
        return false;

    return true;
}

bool Model::InitTextures(ID3D11Device* pDevice, WCHAR* sTextureName)
{
    m_pTexture = new Texture();
    if (!m_pTexture)
        return false;

    if (!m_pTexture->Init(pDevice, sTextureName))
        return false;

    return true;
}

void Model::ShutdownBuffers()
{
    if (m_pIndexBuffer)
    {
        m_pIndexBuffer->Release();
        m_pIndexBuffer = nullptr;
    }
    if (m_pVertexBuffer)
    {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = nullptr;
    }
}

void Model::ShutdownTextures()
{
    if (m_pTexture)
    {
        m_pTexture->Shutdown();
        delete m_pTexture;
        m_pTexture = nullptr;
    }
}

UINT Model::GetVertexCount()
{
    return m_uVertexCount;
}

UINT Model::GetIndexCount()
{
    return m_uIndexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
    return m_pTexture->GetTexture();
}