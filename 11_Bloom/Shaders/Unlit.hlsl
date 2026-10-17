#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VOut VS(VIn vIn)
{
    VOut output;
    
    // Transform position to world space
    float4 worldPos = mul(float4(vIn.position, 1.0), c_modelToWorld);
    
    // Transform to clip space
    output.position = mul(worldPos, c_viewProj);
    
    // Pass through texture coordinates
    output.texCoord = vIn.texCoord;

    // Keep normal for compatibility (but it's useless here)
    output.normal = normalize(mul(float4(vIn.normal, 0.0f), c_modelToWorld)).xyz;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    // Sample the texture
    float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.texCoord);

    // Directly return texture color (no lighting applied)
    return texColor;
}
