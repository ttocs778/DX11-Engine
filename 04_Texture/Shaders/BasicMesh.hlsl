#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;

    output.position = mul(float4(vIn.position, 1.0), c_modelToWorld);
    output.position = mul(output.position, c_viewProj);
    output.color = vIn.color;
    output.texCoord = vIn.texCoord;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.texCoord);
    return pIn.color * texColor;
}
