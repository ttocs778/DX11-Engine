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
    float3 worldPosition : TEXCOORD1;
};

VOut VS(VIn vIn)
{
    VOut output;
    float4 worldPos = mul(float4(vIn.position, 1.0), c_modelToWorld);
    output.worldPosition = worldPos.xyz;
    output.position = mul(worldPos, c_viewProj);

    output.texCoord = vIn.texCoord;
    output.normal = (mul(c_modelToWorld, float4(vIn.normal, 0.0f))).xyz;
    return output;
}



float4 PS(VOut pIn) : SV_TARGET
{
    float3 normal = normalize(pIn.normal);
    float3 accumulatedLight = c_ambient * c_diffuseColor;
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        PointLightData light = c_pointLight[i];
        if (light.isEnabled)
        {
            float3 lightDir = light.position - pIn.worldPosition;
            float distance = length(lightDir);
            lightDir = normalize(lightDir);
            float attenuation = 1.0f - smoothstep(light.innerRadius, light.outerRadius, distance);
            float diffusedLight = max(dot(normal, lightDir), 0.0f);
            float3 diffuseColor = light.lightColor * diffusedLight * c_diffuseColor;
            float3 reflectDir = reflect(-lightDir, normal);
            float3 viewDir = c_cameraPosition - pIn.worldPosition;
            viewDir = normalize(viewDir);
            float spec = max(dot(reflectDir, viewDir), 0.0f);
            spec = pow(spec, c_specularPower);
            float3 specColor = light.lightColor * spec * c_specularColor;
            float3 totalLight = (diffuseColor + specColor) * attenuation;
            accumulatedLight += totalLight;
        }
    }
    float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.texCoord);
    float3 finalColor = texColor.rgb * accumulatedLight;
    return float4(finalColor, texColor.a);
}
 