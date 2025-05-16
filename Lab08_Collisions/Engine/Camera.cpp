#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera()
{
	pGraphics = Graphics::Get();
	worldToCameraMatrix = Matrix4::CreateTranslation(Vector3(500.0f, 0.0f, 0.0f));
	projectionMatrix = Matrix4::CreateRotationY(-Math::PiOver2)
		* Matrix4::CreateRotationZ(-Math::PiOver2)
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
			pGraphics->GetScreenWidth(), pGraphics->GetScreenHeight(),
			25.0f, 10000.0f);

	mConstantBuffer = pGraphics->CreateGraphicsBuffer(nullptr, sizeof(PerCameraConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Camera::~Camera()
{
	mConstantBuffer->Release();
}

void Camera::SetActive()
{
	mCameraConstants.c_viewProj = worldToCameraMatrix * projectionMatrix;
	Matrix4 worldToCameraCopy = worldToCameraMatrix;
	worldToCameraCopy.Invert();
	Vector3 camPos = worldToCameraCopy.GetTranslation();
	mCameraConstants.c_cameraPosition = camPos;
	pGraphics->UploadBuffer(mConstantBuffer, &mCameraConstants, sizeof(mCameraConstants));
	ID3D11DeviceContext* deviceContext = pGraphics->GetDeviceContext();
	deviceContext->VSSetConstantBuffers(pGraphics->CONSTANT_BUFFER_CAMERA, 1, &mConstantBuffer);
	deviceContext->PSSetConstantBuffers(pGraphics->CONSTANT_BUFFER_CAMERA, 1, &mConstantBuffer);
}