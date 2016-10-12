#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Init(ID3D11Device*, WCHAR*);
    void Shutdown();

    ID3D11ShaderResourceView* GetTexture();

private:
    ID3D11ShaderResourceView* m_pTexture;
};


#endif // !TEXTURE_h
