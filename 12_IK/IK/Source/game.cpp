#include "stdafx.h"
#include "Game.h"
#include "Camera.h"
#include "engineMath.h"
#include "Graphics.h"
#include "jsonUtil.h"
#include "material.h"
#include "mesh.h"
#include "RenderCube.h"
#include "RenderObj.h"
#include "Shader.h"
#include "SkinnedObj.h"
#include "stringUtil.h"
#include "texture.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "Components/character.h"
#include "Components/figureEight.h"
#include "Components/followCam.h"
#include "Components/ikChain.h"
#include "Components/player.h"
#include "Components/pointLight.h"


Game::Game()
    : mMouseButtonDown(false)
    , mCamera(nullptr)
    , mLightBuffer(nullptr)
{
    ZeroMemory(&mLights, sizeof(mLights));
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	mGraphics.InitD3D(hWnd, width, height);

    mCamera = new Camera();

    {
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosColor, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
	    Shader* pShader = new Shader();
        pShader->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Mesh", pShader);
    }
    {
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION",   0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexPosColorUVNorm, pos),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",      0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  offsetof(VertexPosColorUVNorm, color),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",   0,  DXGI_FORMAT_R32G32_FLOAT,       0,  offsetof(VertexPosColorUVNorm, uv),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",     0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexPosColorUVNorm, norm),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
	    Shader* pShader = new Shader();
        pShader->Load(L"Shaders/BasicMesh.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"BasicMesh", pShader);
    }
    {
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION",   0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexPosNormUV, pos),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",     0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexPosNormUV, norm),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",   0,  DXGI_FORMAT_R32G32_FLOAT,       0,  offsetof(VertexPosNormUV, uv),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        Shader* pShader = new Shader();
        pShader->Load(L"Shaders/Phong.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Phong", pShader);
        pShader = new Shader();
        pShader->Load(L"Shaders/Unlit.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Unlit", pShader);
    }
    {
        D3D11_INPUT_ELEMENT_DESC inputElem[] =
        {
            { "POSITION",   0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexSkin, pos),      D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",     0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  offsetof(VertexSkin, norm),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONE",       0,  DXGI_FORMAT_R8G8B8A8_UINT,      0,  offsetof(VertexSkin, bones),    D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "WEIGHT",     0,  DXGI_FORMAT_R8G8B8A8_UNORM,     0,  offsetof(VertexSkin, weights),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",   0,  DXGI_FORMAT_R32G32_FLOAT,       0,  offsetof(VertexSkin, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        Shader* pShader = new Shader();
        pShader->Load(L"Shaders/Skinned.hlsl", inputElem, ARRAY_SIZE(inputElem));
        mAssets.SetShader(L"Skinned", pShader);
    }

    {   // Lights
        mLightBuffer = mGraphics.CreateGraphicsBuffer(&mLights, sizeof(mLights), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    }

    LoadLevel(L"Assets/Levels/Final.itplevel");
}

void Game::Shutdown()
{
    for (RenderObj* obj : mRenderObjs)
        delete obj;
    delete mCamera;
    mLightBuffer->Release();
    mAssets.Clear();

    mGraphics.CleanD3D();   // keep this last
}

void Game::Update(float deltaTime)
{
    for (RenderObj* pObj : mRenderObjs)
    {
        pObj->Update(deltaTime);
    }
}

void Game::RenderFrame()
{
	// Set the render target
	mGraphics.SetRenderTarget(mGraphics.GetBackBuffer(), mGraphics.GetDepthStencilView());

	{	// Clear the screen to blue
        mGraphics.ClearDepthBuffer(mGraphics.GetDepthStencilView(), 1.0f);
		Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
		mGraphics.ClearRenderTarget(clearColor);
	}

    {   // Lights
        mGraphics.UploadBuffer(mLightBuffer, &mLights, sizeof(mLights));
        mGraphics.GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTS, 1, &mLightBuffer);
    }

    // camera
    mCamera->SetActive();

    {   // draw the objs
        for (RenderObj* obj : mRenderObjs)
        {
            obj->Draw();
        }
    }

    mGraphics.Debug_DrawLines();

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

void Game::OnMouseMove(int32_t x, int32_t y)
{
    mMousePos = Vector2(x, y);
}

void Game::OnMouseButtonDown() 
{
    if (!mMouseButtonDown)
        mMouseClickPos = mMousePos;
    mMouseButtonDown = true; 
}

void Game::OnMouseButtonUp() 
{ 
    mMouseButtonDown = false; 
}

bool Game::IsMouseButtonHeld() const
{
    return mMouseButtonDown;
}

Vector2 Game::GetRelativeMouse() const
{
    Vector2 move = mMousePos - mMouseClickPos;
    return move * Vector2(1.0f / mGraphics.GetScreenWidth(), 1.0f / mGraphics.GetScreenHeight());
}

Lights::PointLightData* Game::AllocateLight()
{
    for (int i = 0; i < Lights::MAX_POINT_LIGHTS; ++i)
    {   // find the first available light
        Lights::PointLightData* pLight = &mLights.c_pointLight[i];
        if (false == pLight->isEnabled)
        {
            pLight->isEnabled = true;
            return pLight;
        }
    }
    return nullptr; // no lights available
}

void Game::FreeLight(Lights::PointLightData* pLight)
{
    pLight->isEnabled = false;
}

RenderObj* Game::FindObjByName(const std::string& name)
{
    auto found = mNamedObjs.find(name);
    if (found != mNamedObjs.end())
    {
        return found->second;
    }
    return nullptr;
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

    auto& camera = doc["camera"];
    if (camera.IsObject())
    {
        Vector3 pos;
        GetVectorFromJSON(camera, "position", pos);
        Quaternion rot;
        GetQuaternionFromJSON(camera, "rotation", rot);
        Matrix4 camMat = Matrix4::CreateFromQuaternion(rot)
            * Matrix4::CreateTranslation(pos);
        camMat.Invert();
        mCamera->SetViewMat(camMat);
    }

    auto& lighting = doc["lightingData"];
    if (lighting.IsObject())
    {
        Vector3 ambient;
        GetVectorFromJSON(lighting, "ambient", ambient);
        SetAmbientLight(ambient);
    }

    auto& objs = doc["renderObjects"];
    if (objs.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < objs.Size(); i++)
        {
            if (objs[i].IsObject())
            {
                Vector3 pos = Vector3::Zero;
                Quaternion rot = Quaternion::Identity;
                float scale = 1.0f;
                GetVectorFromJSON(objs[i], "position", pos);
                GetQuaternionFromJSON(objs[i], "rotation", rot);
                GetFloatFromJSON(objs[i], "scale", scale);
                Matrix4 mat = Matrix4::CreateScale(scale)
                    * Matrix4::CreateFromQuaternion(rot)
                    * Matrix4::CreateTranslation(pos);
                std::wstring meshName = L"";
                GetWStringFromJSON(objs[i], "mesh", meshName);
                Mesh* mesh = mAssets.LoadMesh(meshName);
                RenderObj* pObj = nullptr;
                SkinnedObj* pSkin = nullptr;
                if (mesh->IsSkinned())
                {
                    pSkin = new SkinnedObj(mesh);
                    pObj = pSkin;
                }
                else
                {
                    pObj = new RenderObj(mesh);
                }
                pObj->mConstants.c_modelToWorld = mat;
                std::string objName;
                if (GetStringFromJSON(objs[i], "name", objName))
                {
                    mNamedObjs[objName] = pObj;
                }

                auto& comps = objs[i]["components"];
                if (comps.IsArray())
                {
                    for (rapidjson::SizeType j = 0; j < comps.Size(); j++)
                    {
                        std::string compType = "";
                        GetStringFromJSON(comps[j], "type", compType);
                        Component* pComp = nullptr;
                        if (compType == "PointLight")
                        {
                            pComp = new PointLight(pObj, this);
                        }
                        else if (compType == "Character")
                        {
                            pComp = new Character(pSkin);
                        }
                        else if (compType == "FigureEight")
                        {
                            pComp = new FigureEight(pObj);
                        }
                        else if (compType == "FollowCam")
                        {
                            pComp = new FollowCam(pObj, this);
                        }
                        else if (compType == "IKChain")
                        {
                            pComp = new IKChain(pObj, this);
                        }
                        else if (compType == "Player")
                        {
                            pComp = new Player(pSkin, this);
                        }

                        if (pComp)
                        {
                            pComp->LoadProperties(comps[j]);
                            pObj->AddComponent(pComp);
                        }
                    }
                }

                mRenderObjs.push_back(pObj);
            }
        }
    }
	return true;
}