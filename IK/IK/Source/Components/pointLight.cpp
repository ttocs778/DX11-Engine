#include "stdafx.h"
#include "Components\PointLight.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"


PointLight::PointLight(RenderObj* renderObj, Game* game)
    : Component(renderObj)
    , mGame(game)
{
    mPointLight = mGame->AllocateLight();
}

PointLight::~PointLight()
{
    mGame->FreeLight(mPointLight);
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
    GetVectorFromJSON(properties, "lightColor", mPointLight->lightColor);
    GetFloatFromJSON(properties, "innerRadius", mPointLight->innerRadius);
    GetFloatFromJSON(properties, "outerRadius", mPointLight->outerRadius);
}

void PointLight::Update(float deltaTime)
{
    mPointLight->position = mOwner->mConstants.c_modelToWorld.GetTranslation();
}
