// We want to use row major matrices
#pragma pack_matrix(row_major)

cbuffer PerCameraConstants : register(b0)
{
    float4x4 c_viewProj;
    float3 c_cameraPosition;
};

struct VIn
{
    float3 position : POSITION0;
    float4 color : COLOR0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

VOut VS(VIn vIn)
{
    VOut output;

    output.position = mul(float4(vIn.position, 1), c_viewProj);
    output.color = vIn.color;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    return pIn.color;
}
