#include "Constants.hlsl"   
struct VIn
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
struct VOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;
    output.pos = float4(vIn.pos, 1.0);
    output.uv = vIn.uv;
    return output;
}



// tweak these weights (must sum ¡Ö1)
static const float weights[5] =
{
    0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f
};



float4 PS(VOut pIn) : SV_TARGET
{
    float2 step = uTexelSize * uDirection;
    float4 c = DiffuseTexture.Sample(DefaultSampler, pIn.uv) * weights[0];
    for (int i = 1; i < 5; ++i)
    {
        c += DiffuseTexture.Sample(DefaultSampler, pIn.uv + step * i) * weights[i];
        c += DiffuseTexture.Sample(DefaultSampler, pIn.uv - step * i) * weights[i];
    }
    return c;
}

