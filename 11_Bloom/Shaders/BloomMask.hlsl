#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 texCoord : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;
    
    output.position = float4(vIn.position, 1.0);

    output.texCoord = vIn.texCoord;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.texCoord);
    float threshold = max(max(texColor.r, texColor.g), texColor.b);
    if (threshold < 0.8)
        return float4(0, 0, 0, texColor.a);
    else
        return texColor;
}
