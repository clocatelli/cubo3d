
#include "TEXTURE.h"
#include "DDSTextureLoader.h"
#include "HR_debug.h"


Texture::Texture()
{
    m_pTexture = nullptr;
}

Texture::~Texture()
{
}

bool Texture::Init(ID3D11Device* pDevice, WCHAR* sTextureName)
{
    HRESULT hr = DirectX::CreateDDSTextureFromFile(pDevice, sTextureName, nullptr, &m_pTexture);
    if (FAILED(hr))
        return false;

    return true;
}

void Texture::Shutdown()
{
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
    return m_pTexture;
}