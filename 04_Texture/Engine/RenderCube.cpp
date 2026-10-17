#include "stdafx.h"
#include "RenderCube.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"


RenderCube::RenderCube():RenderObj(nullptr) {
	
	static VertexPosColorUV cubeVertex[] =
	{

	//-Z Face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
	//Z FACE
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)},
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
	//-X Face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)},
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
	// +X Face
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) , Vector2(1.0f, 0.0f)},
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
	//-Y Face
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)},
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)},
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(0.0, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)},
	//Y Face
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)},
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },

	};
	static uint16_t cubeIndex[] =
	{
		0, 2, 1,  0, 3, 2,
		4, 5, 6,  4, 6, 7,
		8, 9, 10,  8, 10, 11,
		12, 14, 13,  12, 15, 14,
		16, 19, 18,  16, 18, 17,
		20, 21, 22,  20, 22, 23
	};
	mCubeBuffer = new VertexBuffer(
		cubeVertex,
		24,
		sizeof(VertexPosColorUV),
		cubeIndex,
		36,
		2
	);
	mBuffer = mCubeBuffer;
}


RenderCube::~RenderCube() {

}

