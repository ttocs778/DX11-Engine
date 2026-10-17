#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* vertData, uint32_t vertCount, uint32_t vertStride)
{
	uint32_t bufferSize = vertCount * vertStride;
	mBuffer = pGraphics->CreateGraphicsBuffer(vertData, bufferSize, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	mStride = vertStride;
	mVertexCount = vertCount;
	;
}

VertexBuffer::~VertexBuffer()
{
	mBuffer->Release();
}

void VertexBuffer::Draw() const
{
	ID3D11DeviceContext* deviceContext = pGraphics->GetDeviceContext();
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &offset);
	deviceContext->Draw(mVertexCount, 0);
}