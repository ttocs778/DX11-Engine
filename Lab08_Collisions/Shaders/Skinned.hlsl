#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
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
    float4 skinnedPos = float4(0.0, 0.0, 0.0, 0.0);
    float3 skinnedNormal = float3(0.0, 0.0, 0.0);
    uint indices[4];
    indices[0] = vIn.boneIndices.x;
    indices[1] = vIn.boneIndices.y;
    indices[2] = vIn.boneIndices.z;
    indices[3] = vIn.boneIndices.w;

    for (int i = 0; i < 4; i++)
    {
        skinnedPos += mul(float4(vIn.position, 1.0), c_skinMatrix[indices[i]]) * vIn.boneWeights[i];
        skinnedNormal += (mul(float4(vIn.normal, 0.0), c_skinMatrix[indices[i]])).xyz * vIn.boneWeights[i];
    }
    float4 worldPos = mul(skinnedPos, c_modelToWorld);
    output.worldPosition = worldPos.xyz;
    output.position = mul(worldPos, c_viewProj);
    output.texCoord = vIn.texCoord;
    output.normal = normalize((mul(c_modelToWorld, float4(skinnedNormal, 0.0f))).xyz);
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
 