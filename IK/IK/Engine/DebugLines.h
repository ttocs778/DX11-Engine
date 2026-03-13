#pragma once
#include "Graphics.h"
#include "VertexFormat.h"

class Shader;

#ifdef _DEBUG
class DebugLines
{
public:
    DebugLines(Graphics* graphics);
    ~DebugLines();

    void AddLine(const Vector3& start, const Vector3& end, const Graphics::Color4& color);
    void Draw();

private:
    Graphics* mGraphics;
    Shader* mDebugLineShader;
    std::vector<VertexPosColor> mDebugLines;
};
#else
class DebugLines
{
public:
    DebugLines(Graphics* graphics) {}
    ~DebugLines() {}

    void AddLine(const Vector3& start, const Vector3& end, const Graphics::Color4& color) {}
    void Draw() {}
};
#endif