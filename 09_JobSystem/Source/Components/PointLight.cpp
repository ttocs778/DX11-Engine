#include "stdafx.h"
#include "Components/PointLight.h"
#include "RenderObj.h"
#include "engineMath.h"
#include "jsonUtil.h" 

PointLight::PointLight(RenderObj* pObj, Game* pGame)
	: Component(pObj)
	, mLightData(nullptr)
	, mGame(pGame)
{
	mLightData = mGame->AllocateLight();
	if (!mLightData)
	{

	}
}

PointLight::~PointLight()
{
	if (mLightData)
	{
		mGame->FreeLight(mLightData);
		mLightData = nullptr;
	}
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
	if (!mLightData)
		return;

	Vector3 color(1.0f, 1.0f, 1.0f);
	GetVectorFromJSON(properties, "lightColor", color);
	mLightData->lightColor = color;


	float innerR = 0.0f;
	GetFloatFromJSON(properties, "innerRadius", innerR);
	mLightData->innerRadius = innerR;


	float outerR = 0.0f;
	GetFloatFromJSON(properties, "outerRadius", outerR);
	mLightData->outerRadius = outerR;
}

void PointLight::Update(float deltaTime)
{
	if (!mLightData)
		return;

	Matrix4 objMat = mOwner->GetObjectConstants().c_modelToWorld;
	Vector3 objPos = objMat.GetTranslation();

	mLightData->position = objPos;
}
