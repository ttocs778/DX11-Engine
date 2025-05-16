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

struct VertexPosUVIndicesWeightsNormal
{
	Vector3 pos;
	Vector3 normal;
	uint8_t boneIndices[4];
	uint8_t boneWeights[4];
	Vector2 texCoord;
};

struct VertexPosUVNormalMap
{
	Vector3 pos;
	Vector3 normal;
	Vector3 tangent;
	Vector2 texCoord;
};