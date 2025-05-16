#pragma once
#include <d3d11.h>
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer(
		const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
		const void* indexData, uint32_t indexCount, uint32_t indexStride
	);
	~VertexBuffer();
	void Draw() const;

private:
	ID3D11Buffer* mBuffer;
	ID3D11Buffer* mIndexBuffer;
	DXGI_FORMAT mIndexBufferFormat;
	UINT mVertexStride;
	UINT mVertexCount;
	UINT mIndexCount;
	UINT mIndexStride;
	Graphics* pGraphics = Graphics::Get();
};
