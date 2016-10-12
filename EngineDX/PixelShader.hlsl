
Texture2D g_Texture    : register(t0);
SamplerState g_Sampler : register(s0);

struct PixelShaderInput
{
    float4 vPosition : SV_POSITION;
    float4 vTexCoord : TEXCOORD;
};


float4 main(PixelShaderInput input) : SV_TARGET
{
	return g_Texture.Sample(g_Sampler, input.vTexCoord);
}