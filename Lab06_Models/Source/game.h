#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "RenderCube.h"
#include "texture.h"
#include "Lights.h"
#include "assetManager.h"

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

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	Graphics mGraphics;

	RenderObj* mRenderShip = nullptr;

	bool LoadLevel(const WCHAR* fileName);
	AssetManager* mAssetManager = new AssetManager();
	Camera* mCam = nullptr;
	Lights::LightingConstants mLightingConstants;
	ID3D11Buffer* mLightingBuffer = nullptr;
	std::vector<RenderObj*> mRenderObjs;

};