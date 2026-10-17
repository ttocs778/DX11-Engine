#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"


Camera::Camera()
    : mConstBuffer(nullptr)
{
    Graphics* pGraphics = Graphics::Get();

#if 0   // orthographic
    mWorld2Camera = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, -100.f));
    mProjMatrix = Matrix4::CreateOrtho(
        pGraphics->GetScreenWidth(), pGraphics->GetScreenHeight(),
        1000.0f, -1000.0f
    );
#else   // perspective
    mWorld2Camera = Matrix4::CreateTranslation(Vector3(500.0f, 0.0f, 0.f));
    mProjMatrix = Matrix4::CreateRotationY(-Math::PiOver2)
        * Matrix4::CreateRotationZ(-Math::PiOver2)
        * Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
            pGraphics->GetScreenWidth(), pGraphics->GetScreenHeight(),
            25.0f, 10000.0f
        );
#endif

    mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(&mConstants, sizeof(mConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

/*virtual*/ Camera::~Camera()
{
    mConstBuffer->Release();
}

void Camera::SetActive()
{
    Graphics* pGraphics = Graphics::Get();

    mConstants.c_viewProj = mWorld2Camera * mProjMatrix;
    Matrix4 temp = mWorld2Camera;
    temp.Invert();
    mConstants.c_cameraPosition = temp.GetTranslation();
    pGraphics->UploadBuffer(mConstBuffer, &mConstants, sizeof(mConstants));
    pGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);
    pGraphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);
}