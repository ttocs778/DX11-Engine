#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(
	const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
	const void* indexData, uint32_t indexCount, uint32_t indexStride){

	uint32_t bufferSize = vertexCount * vertexStride;
	mBuffer = pGraphics->CreateGraphicsBuffer(vertexData, bufferSize, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	mVertexStride = vertexStride;
	mVertexCount = vertexCount;
	if (indexStride == 2)
	{
		mIndexBufferFormat = DXGI_FORMAT_R16_UINT;
	}
	else
	{
		mIndexBufferFormat = DXGI_FORMAT_R32_UINT;
	}

	mIndexCount = indexCount;
	uint32_t indexBufferSize = indexCount * indexStride;
	mIndexBuffer = Graphics::Get()->CreateGraphicsBuffer(
		indexData,
		indexBufferSize,
		D3D11_BIND_INDEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,      
		D3D11_USAGE_DYNAMIC          
	);
	mIndexStride = indexStride;
}

VertexBuffer::~VertexBuffer()
{
	mBuffer->Release();
	mIndexBuffer->Release();
}

void VertexBuffer::Draw() const
{
	ID3D11DeviceContext* deviceContext = pGraphics->GetDeviceContext();
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mBuffer, &mVertexStride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, mIndexBufferFormat, offset);
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
}