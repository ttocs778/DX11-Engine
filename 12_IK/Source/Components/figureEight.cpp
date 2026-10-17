#include "stdafx.h"
#include "Components\FigureEight.h"
#include "jsonUtil.h"
#include "RenderObj.h"


FigureEight::FigureEight(RenderObj* renderObj)
    : Component(renderObj)
{
}

FigureEight::~FigureEight()
{
}

void FigureEight::LoadProperties(const rapidjson::Value& properties)
{
    GetVectorFromJSON(properties, "range", mRange);
    GetVectorFromJSON(properties, "freq", mFreq);
    mFreq *= Math::ToRadians(1.0f);
    mCenter = mOwner->mConstants.c_modelToWorld;
}

void FigureEight::Update(float deltaTime)
{
    mTimer += mFreq * deltaTime;
    mTimer.x = fmod(mTimer.x, Math::TwoPi);
    mTimer.y = fmod(mTimer.y, Math::TwoPi);
    mTimer.z = fmod(mTimer.z, Math::TwoPi);

    Vector3 offset(sinf(mTimer.x), sinf(mTimer.y), sinf(mTimer.z));
    offset = mRange * offset;
    mOwner->mConstants.c_modelToWorld = mCenter * Matrix4::CreateTranslation(offset);
}
