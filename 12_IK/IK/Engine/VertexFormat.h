#pragma once
#include "Graphics.h"

struct VertexPosColor
{
    Vector3 pos;
    Graphics::Color4 color;
};

struct VertexPosColorUV
{
    Vector3 pos;
    Graphics::Color4 color;
    Vector2 uv;
};

struct VertexPosNormUV
{
    Vector3 pos;
    Vector3 norm;
    Vector2 uv;
};

struct VertexPosColorUVNorm
{
    Vector3 pos;
    Vector3 norm;
    Graphics::Color4 color;
    Vector2 uv;
};

struct VertexSkin
{
    Vector3 pos;
    Vector3 norm;
    uint8_t bones[4];
    uint8_t weights[4];
    Vector2 uv;
};

