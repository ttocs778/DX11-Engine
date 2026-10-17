#pragma once
#include "stdafx.h"
class Graphics;
struct PerCameraConstants
{
	Matrix4 c_viewProj;
	Vector3 c_cameraPosition;
	float padding;
};


class Camera
{
public:
	Camera();
	virtual ~Camera();	
	void SetActive();

	
protected:
	PerCameraConstants mCameraConstants;
	ID3D11Buffer* mConstantBuffer;
	Matrix4 worldToCameraMatrix;
	Matrix4 projectionMatrix;
	Graphics* pGraphics;
};

