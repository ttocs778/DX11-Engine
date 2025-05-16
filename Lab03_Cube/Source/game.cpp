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


Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	mGraphics.InitD3D(hWnd, width, height);
	uint16_t indeciesArray[] = { 0, 1, 2 };
	VertexPosColor vert[] =
	{
		{ Vector3(0.0f, 0.5f, 0.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(-0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	VertexBuffer* vertexBuffer = new VertexBuffer(vert, 3, sizeof(VertexPosColor), indeciesArray, 3, 2);
	mRenderObj = new RenderObj(vertexBuffer);
	mRenderCube = new RenderCube();
	mCam = new Camera();
	// TODO Lab 02c
	mSimpleShader = new Shader();
	D3D11_INPUT_ELEMENT_DESC inputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	mSimpleShader->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem));

}

void Game::Shutdown()
{
	delete mSimpleShader;
	delete mRenderObj;
	delete mCam;
	delete mRenderCube;
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	static float rotationVariable = 0.0;
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
	mGraphics.SetRenderTarget(mGraphics.GetBackBuffer());

	{	// Clear the screen to blue
		Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
		mGraphics.ClearRenderTarget(clearColor);
	}
	mSimpleShader->SetActive();
	mCam->SetActive();
	mRenderObj->Draw();
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