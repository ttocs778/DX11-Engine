#include "stdafx.h"
#include "camera.h"
#include "components\followCam.h"
#include "game.h"
#include "jsonUtil.h"
#include "renderObj.h"
#include "stringUtil.h"


FollowCam::FollowCam(RenderObj* pObj, Game* pGame)
    : Component(pObj)
    , mGame(pGame)
    , mAzimuth(0.0f)
    , mElevation(0.0f)
    , mRadius(100.0f)
    , mAzAtClick(0.0f)
    , mElvAtClick(0.0f)
    , mYawSpeed(1.0f)
    , mPitchSpeed(1.0f)
    , mElvMin(-0.1f)
    , mElvMax(1.0f)
{
}

void FollowCam::LoadProperties(const rapidjson::Value& properties)
{
    GetVectorFromJSON(properties, "offset", mOffset);
    GetVectorFromJSON(properties, "center", mCenter);
    GetFloatFromJSON(properties, "yawSpeed", mYawSpeed);
    GetFloatFromJSON(properties, "pitchSpeed", mPitchSpeed);
    if (GetFloatFromJSON(properties, "minElevation", mElvMin))
        mElvMin = Math::ToRadians(mElvMin);
    if (GetFloatFromJSON(properties, "maxElevation", mElvMax))
        mElvMax = Math::ToRadians(mElvMax);
    mRadius = mOffset.Length();
    Vector3 offsetXY = mOffset;
    offsetXY.z = 0.0f;
    float dxy = offsetXY.Length();
    mAzimuth = atan2f(-mOffset.y, -mOffset.x);
    mElevation = atan2f(mOffset.z, dxy);
}

void FollowCam::Update(float deltaTime)
{
    if (mGame->IsMouseButtonHeld())
    {
        mAzimuth = mAzAtClick + mGame->GetRelativeMouse().x * mYawSpeed;
        mElevation = mElvAtClick + mGame->GetRelativeMouse().y * mPitchSpeed;
        mElevation = Math::Clamp(mElevation, mElvMin, mElvMax);
    }
    else
    {
        mAzAtClick = mAzimuth;
        mElvAtClick = mElevation;
    }
    Vector3 p;
    p.z = mRadius * sinf(mElevation);
    float dxy = mRadius * cosf(mElevation);
    p.x = -dxy * cosf(mAzimuth);
    p.y = -dxy * sinf(mAzimuth);

    Camera* pCamera = mGame->GetCamera();
    Matrix4 mat = Matrix4::CreateRotationY(mElevation)
        * Matrix4::CreateRotationZ(mAzimuth)
        * Matrix4::CreateTranslation(mOwner->mConstants.c_modelToWorld.GetTranslation() + mCenter + p);
    mat.Invert();
    pCamera->SetViewMat(mat);
}