#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "Graphics.h"
#include "Shader.h"
#include "stringUtil.h"
#include "Skeleton.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "VertexFormat.h"
#include "Camera.h"
#include "Material.h"
#include "iostream"
#include "jsonUtil.h"
#include "Components/PointLight.h"
#include "assetManager.h"
#include "SkinnedObj.h"
#include "Components/Character.h"
#include "Components/player.h"
#include "Components/followCam.h"
#include "collisionBox.h"
#include "Job.h"
#include "JobManager.h"

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

	//cam
	mCam = new Camera();

	//Job manager
	JobManager::Get()->Begin();

	//shaders
	mAssetManager->SetShader(L"Mesh", new Shader());
	mAssetManager->SetShader(L"BasicMesh", new Shader());

	Shader* SimpleShader = mAssetManager->GetShader(L"Mesh");
	Shader* TextureShader = mAssetManager->GetShader(L"BasicMesh");
	// Load Phong Shader
	mAssetManager->SetShader(L"Phong", new Shader());
	Shader* phongShader = mAssetManager->GetShader(L"Phong");

	// Load Unlit Shader
	mAssetManager->SetShader(L"Unlit", new Shader());
	Shader* unlitShader = mAssetManager->GetShader(L"Unlit");

	// Load Skeleton
	mAssetManager->LoadSkeleton(L"Assets/Anims/SK_Mannequin.itpskel");
	mAssetManager->LoadAnimation(L"Assets/Anims/ThirdPersonRun.itpanim2");

	//Load Skin

	mAssetManager->SetShader(L"Skinned", new Shader());
	Shader* skinnedShader = mAssetManager->GetShader(L"Skinned");
	// Create Render Objects
	LoadLevel(L"Assets/Levels/Level08.itplevel");

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
	D3D11_INPUT_ELEMENT_DESC inputTextureElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUVNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUVNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColorUVNormal, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosColorUVNormal, texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	TextureShader->Load(L"Shaders/BasicMesh.hlsl", inputTextureElem, ARRAY_SIZE(inputTextureElem));

	D3D11_INPUT_ELEMENT_DESC phongInputLayout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0, offsetof(VertexPosUVNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, offsetof(VertexPosUVNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,  0, offsetof(VertexPosUVNormal, texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	phongShader->Load(L"Shaders/Phong.hlsl", phongInputLayout, ARRAY_SIZE(phongInputLayout));
	unlitShader->Load(L"Shaders/Unlit.hlsl", phongInputLayout, ARRAY_SIZE(phongInputLayout));

	D3D11_INPUT_ELEMENT_DESC skinnedInputLayout[] =
	{

		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  offsetof(VertexPosUVIndicesWeightsNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  offsetof(VertexPosUVIndicesWeightsNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,    0,  offsetof(VertexPosUVIndicesWeightsNormal,boneIndices) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,   0,  offsetof(VertexPosUVIndicesWeightsNormal, boneWeights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,     0,  offsetof(VertexPosUVIndicesWeightsNormal, texCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	skinnedShader->Load(L"Shaders/Skinned.hlsl", skinnedInputLayout, ARRAY_SIZE(skinnedInputLayout));

}

void Game::Shutdown()
{
	for (RenderObj* obj : mRenderObjs)
	{
		delete obj;
	}
	mRenderObjs.clear();
	mAssetManager->Clear();
	delete mCam;
	delete mPhysics;
	mLightingBuffer->Release();
	JobManager::Get()->End();
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	for (RenderObj* obj : mRenderObjs)
	{
		obj->Update(deltaTime);
	}
	JobManager::Get()->WaitForJobs();
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
	mCam->SetActive();
	for (RenderObj* obj : mRenderObjs)
	{
		obj->Draw(); // Draw each object
	}
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

	//set camera
	if (doc.HasMember("camera") && doc["camera"].IsObject())
	{
		const rapidjson::Value& camData = doc["camera"];

		// Parse position as a Vector3 using the helper function
		Vector3 camPos;
		GetVectorFromJSON(camData, "position", camPos);

		// Parse rotation as a Quaternion using the helper function
		Quaternion camRot;
		GetQuaternionFromJSON(camData, "rotation", camRot);

		// Build the Camera-To-World matrix from parsed data
		Matrix4 cameraToWorld = Matrix4::CreateFromQuaternion(camRot) *
			Matrix4::CreateTranslation(camPos);

		// Invert to get World-To-Camera
		Matrix4 worldToCamera = cameraToWorld;
		worldToCamera.Invert();

		// Assign directly to camera's worldToCameraMatrix
		mCam->worldToCameraMatrix = worldToCamera;
	}


	//set lighting data
	if (doc.HasMember("lightingData") && doc["lightingData"].IsObject())
	{
		const rapidjson::Value& lightingData = doc["lightingData"];

		Vector3 ambientColor;

		GetVectorFromJSON(lightingData, "ambient", ambientColor);

		SetAmbientLight(ambientColor);
	}

	if (doc.HasMember("renderObjects") && doc["renderObjects"].IsArray())
	{
		const rapidjson::Value& objects = doc["renderObjects"];
		for (rapidjson::SizeType i = 0; i < objects.Size(); ++i)
		{
			const rapidjson::Value& objData = objects[i];

			Vector3 position;
			GetVectorFromJSON(objData, "position", position);

			Quaternion rotation;
			GetQuaternionFromJSON(objData, "rotation", rotation);

			float scale = 1.0f; // default
			GetFloatFromJSON(objData, "scale", scale);

			std::wstring meshPath;
			GetWStringFromJSON(objData, "mesh", meshPath);
			RenderObj* renderObj = nullptr;
			if (!meshPath.empty())
			{
				Mesh* mesh = mAssetManager->LoadMesh(meshPath);
				if (mesh)
				{
					if (mesh->IsSkinned())
					{
						renderObj = new SkinnedObj(mesh);
					}
					else
					{
						renderObj = new RenderObj(mesh);
					}

					Matrix4 worldTransform = Matrix4::CreateScale(scale)
						* Matrix4::CreateFromQuaternion(rotation)
						* Matrix4::CreateTranslation(position);

					renderObj->GetObjectConstants().c_modelToWorld = worldTransform;

					mRenderObjs.push_back(renderObj);
				}
			}

			if (renderObj && objData.HasMember("components") && objData["components"].IsArray())
			{
				const rapidjson::Value& comps = objData["components"];
				for (rapidjson::SizeType c = 0; c < comps.Size(); ++c)
				{
					const rapidjson::Value& compData = comps[c];

					// Check if "type" is "PointLight"
					if (compData.HasMember("type") && compData["type"].IsString())
					{
						std::string compType = compData["type"].GetString();
						if (compType == "PointLight")
						{
							// Create a new PointLight component
							PointLight* pLight = new PointLight(renderObj, this);

							pLight->LoadProperties(compData);
							renderObj->AddComponent(pLight);
						}
						else if (compType == "Character")
						{
							Character* pCharacter = new Character(renderObj, this);
							pCharacter->LoadProperties(compData);
							renderObj->AddComponent(pCharacter);
						}
						else if (compType == "Player")
						{
							Player* pPlayer = new Player(dynamic_cast<SkinnedObj*>(renderObj), this);
							pPlayer->LoadProperties(compData);
							renderObj->AddComponent(pPlayer);

						}
						else if (compType == "FollowCam")
						{
							FollowCam* pFollowCam = new FollowCam(renderObj, this);
							pFollowCam->LoadProperties(compData);
							renderObj->AddComponent(pFollowCam);
						}
						else if (compType == "CollisionBox")
						{
							CollisionBox* pColBox = new CollisionBox(renderObj, mPhysics);
							pColBox->LoadProperties(compData);
						}
					}
				}
			}
		}
		return true;
	}
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
