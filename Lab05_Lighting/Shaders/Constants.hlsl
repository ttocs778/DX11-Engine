// We want to use row major matrices
#pragma pack_matrix(row_major)

cbuffer PerCameraConstants : register(b0)
{
    float4x4 c_viewProj;
    float3 c_cameraPosition;
};

cbuffer PerObjectConstants : register(b1)
{
    float4x4 c_modelToWorld;
};

SamplerState DefaultSampler : register(s0);
Texture2D DiffuseTexture : register(t0);

cbuffer MaterialConstants : register(b2)
{
    float3 c_diffuseColor;
    float3 c_specularColor;
    float c_specularPower;
};

#define MAX_POINT_LIGHTS 8

struct PointLightData
{
    float3 lightColor;
    float3 position; // 12 bytes
    float innerRadius; // 4 bytes
    float outerRadius; // 4 bytes
    bool isEnabled; // 4 bytes in constant buffer
};

cbuffer LightingConstants : register(b3)
{
    float3 c_ambient; // 12 bytes
    PointLightData c_pointLight[MAX_POINT_LIGHTS];
};

