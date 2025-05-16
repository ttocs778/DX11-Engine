#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : TEXCOORD1;
    float3 tangent : TANGENT0;
};

VOut VS(VIn vIn)
{
    VOut output;
    float4 worldPos = mul(float4(vIn.position, 1.0), c_modelToWorld);
    output.worldPosition = worldPos.xyz;
    output.position = mul(worldPos, c_viewProj);

    output.texCoord = vIn.texCoord;
    output.normal = normalize(mul(float4(vIn.normal, 0.0f), c_modelToWorld).xyz);
    output.tangent = normalize(mul(float4(vIn.tangent, 0.0f), c_modelToWorld).xyz);
    return output;
}



float4 PS(VOut pIn) : SV_TARGET
{
    float3 tangent = normalize(pIn.tangent);
    float3 biTangent = normalize(cross(pIn.normal, pIn.tangent));
    float3 normal = normalize(pIn.normal);
    float3x3 TBN = float3x3(tangent, biTangent, normal);
    float3 nMap = NormalMap.Sample(DefaultSampler, pIn.texCoord).rgb * 2.0f - 1.0f;
    float3 nW = normalize(mul(nMap, TBN));
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
            float diffusedLight = max(dot(nW, lightDir), 0.0f);
            float3 diffuseColor = light.lightColor * diffusedLight * c_diffuseColor;
            float3 reflectDir = reflect(-lightDir, nW);
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
 