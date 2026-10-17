#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"
#include "Material.h"

RenderObj::RenderObj::RenderObj(const Mesh* mesh) {
	mMesh = mesh;
	mObjectBuffer = Graphics::Get()->CreateGraphicsBuffer(nullptr, sizeof(PerObjectConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	mObjectConstants.c_modelToWorld = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
}

RenderObj::~RenderObj()
{
	mObjectBuffer->Release();
	for (Component* comp : mComponents)
	{
		delete comp;
	}
	mComponents.clear();
}

void RenderObj::Draw()
{
	Graphics::Get()->UploadBuffer(mObjectBuffer, &mObjectConstants, sizeof(mObjectConstants));
	ID3D11DeviceContext* deviceContext = Graphics::Get()->GetDeviceContext();
	deviceContext->VSSetConstantBuffers(Graphics::Get()->CONSTANT_BUFFER_RENDEROBJ, 1, &mObjectBuffer);
	mMesh->Draw();
}

void RenderObj::AddComponent(Component* pComp)
{
	mComponents.push_back(pComp);
}

void RenderObj::Update(float deltaTime)
{
	// Update all components
	for (Component* comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}