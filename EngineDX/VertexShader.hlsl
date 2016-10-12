
cbuffer MatrixCB
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
};

struct VertexShaderInput
{
    float4 vPosition : POSITION;
    float4 vTexCoord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output = (PixelShaderInput)0;
    output.vPosition = mul(input.vPosition, mWorld);
    output.vPosition = mul(output.vPosition, mView);
    output.vPosition = mul(output.vPosition, mProj);

    output.vTexCoord = input.vTexCoord;
     
	return output;
}