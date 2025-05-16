#pragma once
#include <d3d11.h>
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer(const void* vertData, uint32_t vertCount, uint32_t vertStride);
	~VertexBuffer();
	void Draw() const;

private:
	ID3D11Buffer* mBuffer;
	UINT mStride;
	UINT mVertexCount;
	Graphics* pGraphics = Graphics::Get();
};
