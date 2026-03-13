#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
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

    output.worldPos = mul(float4(vIn.position, 1.0), c_modelToWorld);
    output.position = mul(output.worldPos, c_viewProj);
    output.normal = mul(float4(vIn.normal, 0.0f), c_modelToWorld);
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
