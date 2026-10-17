#pragma once
#include "engineMath.h"
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
	Vector2 texCoord;

};

struct VertexPosColorUVNormal
{
	Vector3 pos;
	Vector3 normal;
	Graphics::Color4 color;
	Vector2 texCoord;
};

struct VertexPosUVNormal
{
	Vector3 pos;
	Vector3 normal;
	Vector2 texCoord;
};
