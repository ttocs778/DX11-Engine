#include "stdafx.h"
#include "SkinnedObj.h"
#include "Graphics.h"

SkinnedObj::SkinnedObj(const Mesh* mesh)
    : RenderObj(mesh)
{
    Graphics* pGraphics = Graphics::Get();
    for (int i = 0; i < MAX_SKELETON_BONES; ++i)
        mSkinConst.c_skinMatrix[i] = Matrix4::Identity;
    mSkinBuffer = pGraphics->CreateGraphicsBuffer(&mSkinConst, sizeof(mSkinConst), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

SkinnedObj::~SkinnedObj()
{
    mSkinBuffer->Release();
}

void SkinnedObj::Draw() const
{
    Graphics* pGraphics = Graphics::Get();
    pGraphics->UploadBuffer(mSkinBuffer, &mSkinConst, sizeof(mSkinConst));
    pGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_SKIN, 1, &mSkinBuffer);
    RenderObj::Draw();
}