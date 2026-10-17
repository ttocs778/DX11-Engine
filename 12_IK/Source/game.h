#pragma once
#include "assetManager.h"
#include "Graphics.h"
#include "lights.h"

class Camera;
class Material;
class Mesh;
class RenderObj;
class Shader;
class Texture;

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
    void OnMouseMove(int32_t x, int32_t y);
    void OnMouseButtonDown();
    void OnMouseButtonUp();
    bool IsMouseButtonHeld() const;
    Vector2 GetRelativeMouse() const;

    Camera* GetCamera() { return mCamera; }
    Lights::PointLightData* AllocateLight();
    void FreeLight(Lights::PointLightData* pLight);
    void SetAmbientLight(const Vector3& color) { mLights.c_ambient = color; }
    const Vector3 &GetAmbientLight() const { return mLights.c_ambient; }

    RenderObj* FindObjByName(const std::string& name);

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
    bool mMouseButtonDown;
    Vector2 mMouseClickPos;
    Vector2 mMousePos;
	Graphics mGraphics;
    AssetManager mAssets;
    Camera* mCamera;

    Lights::LightingConstants mLights;
    ID3D11Buffer* mLightBuffer;

    std::vector<RenderObj*> mRenderObjs;
    std::unordered_map<std::string, RenderObj*> mNamedObjs;

	bool LoadLevel(const WCHAR* fileName);
};