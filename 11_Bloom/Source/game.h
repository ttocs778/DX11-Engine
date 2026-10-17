#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "RenderCube.h"
#include "texture.h"
#include "Lights.h"
#include "assetManager.h"
#include "Physics.h"

class Material;
class Shader;
class Camera;
class Game
{
public:
	Game();
	~Game();

	void Init(HWND hWnd, float width, float height);
	void Shutdown();
	void Update(float deltaTime);
	void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;
	Lights::PointLightData* AllocateLight();
	void FreeLight(Lights::PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight() const;
	AssetManager* GetAssetManager() { return mAssetManager; }
	Camera* mCam = nullptr;
	Physics* GetPhysics() { return mPhysics; }
private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	Graphics mGraphics;
	bool LoadLevel(const WCHAR* fileName);
	AssetManager* mAssetManager = new AssetManager();
	Lights::LightingConstants mLightingConstants;
	ID3D11Buffer* mLightingBuffer = nullptr;
	std::vector<RenderObj*> mRenderObjs;
	Physics* mPhysics = new Physics();
	Texture* mOffscreenTexture = nullptr;
	ID3D11RenderTargetView* mOffscreenRTV = nullptr;
	Texture* mHalfResTex = nullptr;
	ID3D11RenderTargetView* mHalfResRTV = nullptr;
	Texture* mQuarterResTex1 = nullptr;
	ID3D11RenderTargetView* mQuarterResRTV1 = nullptr;
	Texture* mQuarterResTex2 = nullptr;
	ID3D11RenderTargetView* mQuarterResRTV2 = nullptr;
	VertexBuffer* mCopyVB;
	float mScreenW = 0.0f;
	float mScreenH = 0.0f;
	ID3D11Buffer* mBlurCB = nullptr;
	ID3D11BlendState* mBlendOpaque = nullptr;
	ID3D11BlendState* mBlendAdditive = nullptr;
};