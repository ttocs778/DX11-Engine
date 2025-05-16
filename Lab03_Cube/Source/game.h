#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "RenderCube.h"

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

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	Graphics mGraphics;

	// TODO Lab 02b
	RenderObj* mRenderObj = nullptr;
	RenderObj* mRenderCube = nullptr;

	// TODO Lab 02c
	Shader* mSimpleShader = nullptr;

	// TODO Lab 02e

	// TODO Lab 02f

	bool LoadLevel(const WCHAR* fileName);

	Camera* mCam = nullptr;
};