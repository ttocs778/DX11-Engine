#pragma once

class VertexBuffer
{
public:
    VertexBuffer(const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
		const void* indexData, uint32_t indexCount, uint32_t indexStride);
    ~VertexBuffer();

    void Draw() const;

private:
    ID3D11Buffer* mVertexBuffer;
    ID3D11Buffer* mIndexBuffer;
    UINT mStride;
    UINT mNumIndex;
    DXGI_FORMAT mFormat;
};