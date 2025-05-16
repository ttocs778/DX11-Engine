#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"


RenderObj::RenderObj(const VertexBuffer* vertexBuffer)
{
	mBuffer = vertexBuffer;
	mObjectBuffer = Graphics::Get()->CreateGraphicsBuffer(nullptr, sizeof(PerObjectConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	mObjectConstants.c_modelToWorld = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
}

RenderObj::~RenderObj()
{
	delete mBuffer;
	mObjectBuffer->Release();
}

void RenderObj::Draw()
{
	Graphics::Get()->UploadBuffer(mObjectBuffer, &mObjectConstants, sizeof(mObjectConstants));
	ID3D11DeviceContext* deviceContext = Graphics::Get()->GetDeviceContext();
	deviceContext->VSSetConstantBuffers(Graphics::Get()->CONSTANT_BUFFER_RENDEROBJ, 1, &mObjectBuffer);
	mBuffer->Draw();
}

