#include "stdafx.h"
#include "RenderCube.h"
#include "Graphics.h"
#include "mesh.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"


static VertexPosColorUVNorm s_cubeVertex[] =
{
    // Red
	{ Vector3(-0.5f,    0.5f,  -0.5f),    Vector3( 0.0f,  0.0f, -1.0f),  Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,  -0.5f),    Vector3( 0.0f,  0.0f, -1.0f),  Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3( 0.5f,   -0.5f,  -0.5f),    Vector3( 0.0f,  0.0f, -1.0f),  Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f,   -0.5f,  -0.5f),    Vector3( 0.0f,  0.0f, -1.0f),  Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),   Vector2(0.0f, 1.0f) },
                                                                                                                            
    // Green                                                                                                                
	{ Vector3(-0.5f,    0.5f,   0.5f),    Vector3( 0.0f,  0.0f,  1.0f),	 Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,   0.5f),    Vector3( 0.0f,  0.0f,  1.0f),	 Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3( 0.5f,   -0.5f,   0.5f),    Vector3( 0.0f,  0.0f,  1.0f),	 Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f,   -0.5f,   0.5f),    Vector3( 0.0f,  0.0f,  1.0f),	 Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),   Vector2(0.0f, 1.0f) },
                                                                                                                            
    // Blue                                                                                                                 
	{ Vector3(-0.5f,   -0.5f,   0.5f),    Vector3(-1.0f,  0.0f,  0.0f),  Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3(-0.5f,    0.5f,   0.5f),    Vector3(-1.0f,  0.0f,  0.0f),  Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3(-0.5f,    0.5f,  -0.5f),    Vector3(-1.0f,  0.0f,  0.0f),  Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f,   -0.5f,  -0.5f),    Vector3(-1.0f,  0.0f,  0.0f),  Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),   Vector2(0.0f, 1.0f) },
                                                                                                                            
    // Yellow                                                                                                               
	{ Vector3( 0.5f,   -0.5f,   0.5f),    Vector3( 1.0f,  0.0f,  0.0f),  Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,   0.5f),    Vector3( 1.0f,  0.0f,  0.0f),  Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,  -0.5f),    Vector3( 1.0f,  0.0f,  0.0f),  Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3( 0.5f,   -0.5f,  -0.5f),    Vector3( 1.0f,  0.0f,  0.0f),  Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f),   Vector2(0.0f, 1.0f) },
                                                                                                                     
    // Cyan                                                                                                          
	{ Vector3(-0.5f,   -0.5f,   0.5f),    Vector3( 0.0f, -1.0f,  0.0f),  Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3( 0.5f,   -0.5f,   0.5f),    Vector3( 0.0f, -1.0f,  0.0f),  Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3( 0.5f,   -0.5f,  -0.5f),    Vector3( 0.0f, -1.0f,  0.0f),  Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f,   -0.5f,  -0.5f),    Vector3( 0.0f, -1.0f,  0.0f),  Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f),   Vector2(0.0f, 1.0f) },
                                                                                                                     
    // Purple                                                                                                        
	{ Vector3(-0.5f,    0.5f,   0.5f),    Vector3( 0.0f,  1.0f,  0.0f),  Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f),   Vector2(0.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,   0.5f),    Vector3( 0.0f,  1.0f,  0.0f),  Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f),   Vector2(1.0f, 0.0f) },
	{ Vector3( 0.5f,    0.5f,  -0.5f),    Vector3( 0.0f,  1.0f,  0.0f),  Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f),   Vector2(1.0f, 1.0f) },
	{ Vector3(-0.5f,    0.5f,  -0.5f),    Vector3( 0.0f,  1.0f,  0.0f),  Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f),   Vector2(0.0f, 1.0f) },
};

static uint16_t s_cubeIndex[] =
{
    // Red
	2, 1, 0,
	3, 2, 0,

    // Green
	4, 5, 6,
	4, 6, 7,

	// Blue
	10, 9, 8,
	11, 10, 8,

    // Yellow
	12, 13, 14,
	12, 14, 15,

    // Cyan
	16, 17, 18,
	16, 18, 19,

    // Purple
	22, 21, 20,
	23, 22, 20,
};

RenderCube::RenderCube(Material* material)
    : RenderObj(nullptr)
{
    VertexBuffer* vert = new VertexBuffer(s_cubeVertex, ARRAY_SIZE(s_cubeVertex), sizeof(s_cubeVertex[0]),
        s_cubeIndex, ARRAY_SIZE(s_cubeIndex), sizeof(s_cubeIndex[0]));
	mMesh = new Mesh(vert, material);
}

/*virtual*/ RenderCube::~RenderCube()
{
	delete mMesh;
}