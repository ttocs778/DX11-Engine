#include "stdafx.h"
#include "SkinnedObj.h"
#include "Graphics.h" 
#include <cassert>
#include <cstring>


SkinnedObj::SkinnedObj(const Mesh* mesh) :
	RenderObj(mesh), mSkinBuffer(nullptr)
{
	for (int i = 0; i < MAX_SKELETON_BONES; i++) {
		skinConstants.c_skinMatrix[i] = Matrix4::Identity;
	}
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(SkinConstants);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &mSkinBuffer);
	assert(SUCCEEDED(hr) && "Failed to create skin constant buffer");
}

SkinnedObj::~SkinnedObj()
{
	if (mSkinBuffer) {
		mSkinBuffer->Release();
		mSkinBuffer = nullptr;
	}

}

void SkinnedObj::Draw()
{
	Graphics::Get()->UploadBuffer(mSkinBuffer, &skinConstants, sizeof(SkinConstants));
	ID3D11DeviceContext* deviceContext = Graphics::Get()->GetDeviceContext();
	deviceContext->VSSetConstantBuffers(Graphics::Get()->CONSTANT_BUFFER_SKIN, 1, &mSkinBuffer);
	RenderObj::Draw();
}


