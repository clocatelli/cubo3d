#ifndef MODEL_H
#define MODEL_H

#include "TEXTURE.h"

#include <d3d11.h>
#include <DirectXMath.h>


struct Vertex
{
    DirectX::XMFLOAT4 vPosition;
    DirectX::XMFLOAT2 vTexCoord;
};

class Model
{
public:
    Model();
    ~Model();

    bool Init(ID3D11Device*, WCHAR*);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    UINT GetVertexCount();
    UINT GetIndexCount();
    ID3D11ShaderResourceView* GetTexture();
private:
    bool InitBuffers(ID3D11Device*);
    bool InitTextures(ID3D11Device*, WCHAR*);
    void ShutdownBuffers();
    void ShutdownTextures();

private:
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    UINT m_uVertexCount;
    UINT m_uIndexCount;

    Texture* m_pTexture;
};



#endif // !MODEL_H
