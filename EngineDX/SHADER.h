#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>

struct MatricesCB
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProj;
};

class Shader
{
public:
    Shader();
    ~Shader();

    bool Init(ID3D11Device*, WCHAR*, WCHAR*);
    void Shutdown();
    void SetShaderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*, MatricesCB*);
    void Render(ID3D11DeviceContext*, UINT);

private:
    bool InitShaders(ID3D11Device*, WCHAR*, WCHAR*);
    bool InitSamplers(ID3D11Device*);
    bool InitConstantBuffers(ID3D11Device*);
    void ShutdownShaders();
    void ShutdownSamplers();
    void ShutdownConstantBuffers();

private:
    ID3D11PixelShader* m_pPixelShader;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11InputLayout* m_pInputLayout;

    ID3D11SamplerState* m_pSamplerState;
    ID3D11Buffer* m_pMatricesCB;
};



#endif // !SHADER_H
