#include "stdafx.h"
#include "DebugLines.h"
#include "Shader.h"

#ifdef _DEBUG

DebugLines::DebugLines(Graphics* graphics)
    : mGraphics(graphics)
    , mDebugLineShader(nullptr)
{
    D3D11_INPUT_ELEMENT_DESC inputElem[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosColor, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    mDebugLineShader = new Shader();
    mDebugLineShader->Load(L"Engine/Shaders/DebugLine.hlsl", inputElem, ARRAY_SIZE(inputElem));
}

DebugLines::~DebugLines()
{
    delete mDebugLineShader;
}

void DebugLines::AddLine(const Vector3& start, const Vector3& end, const Graphics::Color4& color)
{
    mDebugLines.push_back(VertexPosColor{ start, color });
    mDebugLines.push_back(VertexPosColor{ end, color });
}

void DebugLines::Draw()
{
    mGraphics->SetRenderTarget(mGraphics->GetBackBuffer(), nullptr);

    mGraphics->GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    UINT count = (UINT)mDebugLines.size();
    if (count > 0)
    {
        mDebugLineShader->SetActive();
        ID3D11Buffer* vertBuffer = mGraphics->CreateGraphicsBuffer(&mDebugLines[0], stride * count,
            D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
        mGraphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
        mGraphics->GetDeviceContext()->Draw(count, 0);
        vertBuffer->Release();
    }
    mGraphics->GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mDebugLines.clear();
}

#endif