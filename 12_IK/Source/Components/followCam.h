#pragma once
#include "component.h"
#include "engineMath.h"

class Game;

class FollowCam : public Component
{
public:
    FollowCam(RenderObj* pObj, Game* pGame);

    void LoadProperties(const rapidjson::Value& properties) override;

    void Update(float deltaTime) override;

private:
    Vector3 mOffset;
    Vector3 mCenter;
    Game* mGame;
    float mAzimuth;
    float mElevation;
    float mRadius;
    float mAzAtClick;
    float mElvAtClick;

    float mYawSpeed;
    float mPitchSpeed;
    float mElvMin;
    float mElvMax;
};