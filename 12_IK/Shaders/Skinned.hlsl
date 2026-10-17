#include "Constants.hlsl"

#define MAX_SKELETON_BONES 80
cbuffer SkinConstants : register(b4)
{
    float4x4 c_skinMatrix[MAX_SKELETON_BONES];
};

struct VIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    uint4 bones : BONE0;
    float4 weights : WEIGHT0;
    float2 uv : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION1;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

VOut VS(VIn vIn)
{
    VOut output;

    float4 pos = float4(vIn.position, 1.0);
    float4 norm = float4(vIn.normal, 0.0f);
#if 1
    pos = vIn.weights.x * mul(pos, c_skinMatrix[vIn.bones.x])
        + vIn.weights.y * mul(pos, c_skinMatrix[vIn.bones.y])
        + vIn.weights.z * mul(pos, c_skinMatrix[vIn.bones.z])
        + vIn.weights.w * mul(pos, c_skinMatrix[vIn.bones.w]);
    norm = vIn.weights.x * mul(norm, c_skinMatrix[vIn.bones.x])
        + vIn.weights.y * mul(norm, c_skinMatrix[vIn.bones.y])
        + vIn.weights.z * mul(norm, c_skinMatrix[vIn.bones.z])
        + vIn.weights.w * mul(norm, c_skinMatrix[vIn.bones.w]);
#endif
    output.worldPos = mul(pos, c_modelToWorld);
    output.position = mul(output.worldPos, c_viewProj);
    output.normal = mul(norm, c_modelToWorld);
    output.uv = vIn.uv;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float3 n = pIn.normal.xyz;
    n = normalize(n);

    float3 v = c_cameraPosition - pIn.worldPos.xyz;
    v = normalize(v);
    
    float3 light = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (c_pointLight[i].isEnabled)
        {
            float3 l = c_pointLight[i].position - pIn.worldPos.xyz;
            float dist = length(l);
            l = l / dist;
            dist = smoothstep(c_pointLight[i].outerRadius, c_pointLight[i].innerRadius, dist);
            
            // diffuse
            float3 d = dot(l, n);
            d = max(d, 0.0f);
            light += dist * d * c_pointLight[i].lightColor * c_diffuseColor;
            
            // specular
            float3 r = reflect(-l, n);
            d = dot(r, v);
            d = max(d, 0.0f);
            d = pow(d, c_specularPower);
            light += dist * d * c_pointLight[i].lightColor * c_specularColor;
        }

    }
    
    light += c_ambient;
    
    float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.uv);
    return float4(light, 1.0f) * texColor;
}
