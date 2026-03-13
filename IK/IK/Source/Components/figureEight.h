#pragma once
#include "Component.h"
#include "engineMath.h"


class FigureEight : public Component
{
public:
    FigureEight(RenderObj* renderObj);
    ~FigureEight();

    void LoadProperties(const rapidjson::Value& properties) override;
    void Update(float deltaTime) override;

private:
    Vector3 mRange;
    Vector3 mFreq;
    Matrix4 mCenter;
    Vector3 mTimer;
};