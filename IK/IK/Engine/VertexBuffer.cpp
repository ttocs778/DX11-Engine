#include "stdafx.h"
#include "VertexBuffer.h"
#include "Graphics.h"


VertexBuffer::VertexBuffer(const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
		const void* indexData, uint32_t indexCount, uint32_t indexStride)
    : mStride(vertexStride)
    , mNumIndex(indexCount)
{
    Graphics *pGraphics = Graphics::Get();
    mVertexBuffer = pGraphics->CreateGraphicsBuffer(vertexData, mStride * vertexCount, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    mIndexBuffer = pGraphics->CreateGraphicsBuffer(indexData, indexStride * indexCount, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    mFormat = indexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
}

VertexBuffer::~VertexBuffer()
{
    mVertexBuffer->Release();
    mIndexBuffer->Release();
}

void VertexBuffer::Draw() const
{
    Graphics *pGraphics = Graphics::Get();

    UINT offset = 0;
	pGraphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &offset);
	pGraphics->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, mFormat, 0);
    pGraphics->GetDeviceContext()->DrawIndexed(mNumIndex, 0, 0);
}