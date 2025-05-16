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
{
	Matrix4 mat = mOwner->GetObjectConstants().c_modelToWorld;
	mat = Matrix4::CreateRotationX(Math::PiOver2) * mat;	//mrwTODO this 90 rotation accounts for the orientation of the model :(
}

void FollowCam::LoadProperties(const rapidjson::Value& properties)
{
	Component::LoadProperties(properties);
	GetVectorFromJSON(properties, "offset", mOffset);
}

void FollowCam::Update(float deltaTime)
{
	Camera* pCamera = mGame->mCam;
	Matrix4 mat = Matrix4::CreateTranslation(mOwner->GetObjectConstants().c_modelToWorld.GetTranslation() + mOffset);
	mat.Invert();
	pCamera->worldToCameraMatrix = mat;
}