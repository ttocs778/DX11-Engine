#include "stdafx.h"
#include "RenderObj.h"
#include "Component.h"
#include "engineMath.h"
#include "Graphics.h"
#include "mesh.h"

RenderObj::RenderObj(const Mesh* mesh)
    : mMesh(mesh)
{
    Graphics* pGraphics = Graphics::Get();

    mConstants.c_modelToWorld = Matrix4::Identity;
    mConstants.c_modelToWorld = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
    mConstBuffer = pGraphics->CreateGraphicsBuffer(&mConstants, sizeof(mConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

RenderObj::~RenderObj()
{
    mConstBuffer->Release();
    for (Component* comp : mComponents)
    {
        delete comp;
    }
}

void RenderObj::AddComponent(Component* pComp)
{
    mComponents.push_back(pComp);
}

/*virtual*/ void RenderObj::Draw() const
{
    if (mMesh)
    {
        Graphics* pGraphics = Graphics::Get();
        pGraphics->UploadBuffer(mConstBuffer, &mConstants, sizeof(mConstants));
        pGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_RENDEROBJ, 1, &mConstBuffer);
        mMesh->Draw();
    }
}

/*virtual*/ void RenderObj::Update(float deltaTime)
{
    for (Component* comp : mComponents)
    {
        comp->Update(deltaTime);
    }
}
