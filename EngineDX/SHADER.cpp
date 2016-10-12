#include "SHADER.h"

#include <D3Dcompiler.h>

Shader::Shader()
{
    m_pPixelShader = nullptr;
    m_pVertexShader = nullptr;
    m_pInputLayout = nullptr;

    m_pSamplerState = nullptr;
    m_pMatricesCB = nullptr;
}

Shader::~Shader()
{
}

bool Shader::Init(ID3D11Device* pDevice, WCHAR* sVSName, WCHAR* sPSName)
{
    if (!InitShaders(pDevice, sVSName, sPSName))
        return false;

    if (!InitSamplers(pDevice))
        return false;

    if (!InitConstantBuffers(pDevice))
        return false;

    return true;
}

void Shader::Shutdown()
{
    ShutdownShaders();
    ShutdownSamplers();
    ShutdownConstantBuffers();
}

void Shader::SetShaderParameters(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView* pTexture, MatricesCB* pMatricesCB)
{
    pContext->UpdateSubresource(m_pMatricesCB, 0, 0, pMatricesCB, 0, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_pMatricesCB);

    pContext->PSSetShaderResources(0, 1, &pTexture);
}

void Shader::Render(ID3D11DeviceContext* pContext, UINT uIndexCount)
{
    pContext->IASetInputLayout(m_pInputLayout);
    pContext->VSSetShader(m_pVertexShader, 0, 0);
    pContext->PSSetShader(m_pPixelShader, 0, 0);
    // SAMPLER
    pContext->PSSetSamplers(0, 1, &m_pSamplerState);

    pContext->DrawIndexed(uIndexCount, 0, 0);
}

bool Shader::InitShaders(ID3D11Device* pDevice, WCHAR* sVSName, WCHAR* sPSName)
{
    ID3DBlob* pVSBuffer = nullptr;
    ID3DBlob* pErrorBuffer = nullptr;
    HRESULT hr = D3DCompileFromFile(sVSName, nullptr, nullptr, "main", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &pVSBuffer, &pErrorBuffer);

    if (FAILED(hr))
    {
        if (pErrorBuffer)
        {
            OutputDebugStringA((LPCSTR)pErrorBuffer->GetBufferPointer());
            pErrorBuffer->Release();
            pErrorBuffer = nullptr;
        }
        return false;
    }

    ID3DBlob* pPSBuffer = nullptr;
    hr = D3DCompileFromFile(sPSName, nullptr, nullptr, "main", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &pPSBuffer, &pErrorBuffer);

    if (FAILED(hr))
    {
        if (pErrorBuffer)
        {
            OutputDebugStringA((LPCSTR)pErrorBuffer->GetBufferPointer());
            pErrorBuffer->Release();
            pErrorBuffer = nullptr;
        }
        return false;
    }

    hr = pDevice->CreateVertexShader(pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), 0, &m_pVertexShader);
    if (FAILED(hr))
        return false;

    hr = pDevice->CreatePixelShader(pPSBuffer->GetBufferPointer(), pPSBuffer->GetBufferSize(), 0, &m_pPixelShader);
    if (FAILED(hr))
        return false;

    D3D11_INPUT_ELEMENT_DESC layout[2];
    layout[0].SemanticName = "POSITION";
    layout[0].SemanticIndex = 0;
    layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[0].InputSlot = 0;
    layout[0].AlignedByteOffset = 0;
    layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[0].InstanceDataStepRate = 0;

    layout[1].SemanticName = "TEXCOORD";
    layout[1].SemanticIndex = 0;
    layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    layout[1].InputSlot = 0;
    layout[1].AlignedByteOffset = 16;
    layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[1].InstanceDataStepRate = 0;

    UINT uNumElements = ARRAYSIZE(layout);

    hr = pDevice->CreateInputLayout(layout, uNumElements, pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(), &m_pInputLayout);

    if (FAILED(hr))
        return false;

    pVSBuffer->Release();
    pPSBuffer->Release();
    pVSBuffer = nullptr;
    pPSBuffer = nullptr;


    return true;
} 

bool Shader::InitSamplers(ID3D11Device* pDevice)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MinLOD = 0;
    samplerDesc.MipLODBias = 0;

    HRESULT hr = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
    if (FAILED(hr))
        return false;

    return true;
}

void Shader::ShutdownShaders()
{
    if (m_pInputLayout)
    {
        m_pInputLayout->Release();
        m_pInputLayout = nullptr;
    }

    if (m_pPixelShader)
    {
        m_pPixelShader->Release();
        m_pPixelShader = nullptr;
    }

    if (m_pVertexShader)
    {
        m_pVertexShader->Release();
        m_pVertexShader = nullptr;
    }
}

void Shader::ShutdownSamplers()
{
    if (m_pSamplerState)
    {
        m_pSamplerState->Release();
        m_pSamplerState = nullptr;
    }
}

bool Shader::InitConstantBuffers(ID3D11Device* pDevice)
{
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.ByteWidth = sizeof(MatricesCB);
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    cbDesc.Usage = D3D11_USAGE_DEFAULT;

    HRESULT hr = pDevice->CreateBuffer(&cbDesc, nullptr, &m_pMatricesCB);
    if (FAILED(hr))
        return false;

    return true;
}

void Shader::ShutdownConstantBuffers()
{
    if (m_pMatricesCB)
    {
        m_pMatricesCB->Release();
        m_pMatricesCB = nullptr;
    }
}