#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "Graphics.h"
#include "Shader.h"
#include "stringUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "VertexFormat.h"
#include "Camera.h"
#include "Material.h"
#include "iostream"


Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	//initlize screen
	mGraphics.InitD3D(hWnd, width, height);

	//create texture
	mTexture = new Texture();
	mTexture->Load(L"Assets/Textures/Cube.png");
	uint16_t indeciesArray[] = { 0, 1, 2 };
	VertexPosColor vert[] =
	{
		{ Vector3(0.0f, 0.5f, 0.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(-0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	//buffers
	VertexBuffer* vertexBuffer = new VertexBuffer(vert, 3, sizeof(VertexPosColor), indeciesArray, 3, 2);
	mRenderObj = new RenderObj(vertexBuffer);
	mRenderCube = new RenderCube();

	//cam
	mCam = new Camera();

	//shaders
	mSimpleShader = new Shader();
	mTextureShader = new Shader();

	//material
	mTriangleMaterial = new Material();
	mTriangleMaterial->SetShader(mSimpleShader);
	mCubeMaterial = new Material();
	mCubeMaterial->SetShader(mTextureShader);
	mCubeMaterial->SetTexture(Graphics::TEXTURE_SLOT_DIFFUSE, mTexture);
	mCubeMaterial->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
	mCubeMaterial->SetSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
	mCubeMaterial->SetSpecularPower(10.0f);
	SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));

	//light1
	Lights::PointLightData* pointLight1 = AllocateLight();
	pointLight1->lightColor = Vector3(0.0f, 0.0f, 1.0f);
	pointLight1->position = Vector3(-100.0f, 0.0f, 100.0f);
	pointLight1->innerRadius = 20.0f;
	pointLight1->outerRadius = 200.0f;
	//light2
	Lights::PointLightData* pointLight2 = AllocateLight();;
	pointLight2->lightColor = Vector3(0.0f, 1.0f, 0.0f);
	pointLight2->position = Vector3(0.0f, 150.0f, 0.0f);
	pointLight2->innerRadius = 20.0f;
	pointLight2->outerRadius = 200.0f;
	//light3
	Lights::PointLightData* pointLight3 = AllocateLight();;
	pointLight3->lightColor = Vector3(1.0f, 0.0f, 0.0f);
	pointLight3->position = Vector3(100.0f, 0.0f, 100.0f);
	pointLight3->innerRadius = 20.0f;
	pointLight3->outerRadius = 200.0f;

	//creating lighting buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Lights::LightingConstants);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &mLightingConstants;
	mGraphics.GetDevice()->CreateBuffer(&bufferDesc, &initData, &mLightingBuffer);

	//set shader
	D3D11_INPUT_ELEMENT_DESC inputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	mSimpleShader->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem));

	D3D11_INPUT_ELEMENT_DESC inputTextureElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUVNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColorUVNormal, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosColorUVNormal, texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUVNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mTextureShader->Load(L"Shaders/BasicMesh.hlsl", inputTextureElem, ARRAY_SIZE(inputTextureElem));
}

void Game::Shutdown()
{
	delete mSimpleShader;
	delete mRenderObj;
	delete mCam;
	delete mRenderCube;
	delete mTexture;
	delete mTextureShader;
	delete mCubeMaterial;
	delete mTriangleMaterial;
	mLightingBuffer->Release();
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	static float rotationVariable = 0.0f;
	rotationVariable += Math::Pi * deltaTime;
	Matrix4 scaleMatrix = Matrix4::CreateScale(300.0f);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(rotationVariable);
	Matrix4 modelToWorld = scaleMatrix * rotationMatrix;
	mRenderObj->GetObjectConstants().c_modelToWorld = modelToWorld;
	Matrix4 cubeScaleMatrix = Matrix4::CreateScale(100.0f);
	Matrix4 cubeRotationMatrixY = Matrix4::CreateRotationY(rotationVariable);
	Matrix4 cubeRotationMatrixX = Matrix4::CreateRotationX(0.25f * rotationVariable);
	Matrix4 cubeToWorld = cubeScaleMatrix * cubeRotationMatrixY * cubeRotationMatrixX;
	mRenderCube->GetObjectConstants().c_modelToWorld = cubeToWorld;
}

void Game::RenderFrame()
{
	// Set the render target
	mGraphics.SetRenderTarget(mGraphics.GetBackBuffer(), mGraphics.GetDepthSencilView());

	{	// Clear the screen to blue
		Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
		mGraphics.ClearRenderTarget(clearColor);
		mGraphics.ClearDepthBuffer(mGraphics.GetDepthSencilView(), 1.0f);
	}

	mGraphics.GetDeviceContext()->UpdateSubresource(
		mLightingBuffer, 0, nullptr, &mLightingConstants, 0, 0);

	mGraphics.GetDeviceContext()->PSSetConstantBuffers(
		Graphics::CONSTANT_BUFFER_LIGHTING, 1, &mLightingBuffer);
	mTriangleMaterial->SetActive();
	mCam->SetActive();
	mRenderObj->Draw();
	mCubeMaterial->SetActive();
	mRenderCube->Draw();
	mGraphics.EndFrame();
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	return true;
}


Lights::PointLightData* Game::AllocateLight()
{
	for (int i = 0; i < Lights::pointLightNum; ++i)
	{
		Lights::PointLightData& light = mLightingConstants.c_pointLight[i];
		if (!light.isEnabled)
		{
			light.isEnabled = true;
			return &light;
		}
	}
	return nullptr;
}

void Game::FreeLight(Lights::PointLightData* pLight)
{
	if (pLight)
	{
		pLight->isEnabled = false;
	}
}

void Game::SetAmbientLight(const Vector3& color)
{
	mLightingConstants.c_ambient = color;
}

const Vector3& Game::GetAmbientLight() const
{
	return mLightingConstants.c_ambient;
}
