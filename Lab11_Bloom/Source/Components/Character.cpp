#include "stdafx.h"
#include "Character.h"
#include "RenderObj.h"
#include "assetManager.h"
#include "AnimationJob.h"
#include "JobManager.h"
Character::Character(RenderObj* pObj, Game* pGame) :
	Component(pObj),
	mSkinnedObj(nullptr),
	mSkeleton(nullptr),
	mCurrentAnim(nullptr),
	mCurrentTime(0.0f),
	mGame(pGame)
{
	mSkinnedObj = dynamic_cast<SkinnedObj*>(pObj);
}

Character::~Character()
{

}

void Character::LoadProperties(const rapidjson::Value& properties)
{
	AssetManager* assetManager = mGame->GetAssetManager();
	if (!assetManager)
		return;
	std::wstring skeletonStr;
	if (GetWStringFromJSON(properties, "skeleton", skeletonStr))
	{
		mSkeleton = assetManager->LoadSkeleton(skeletonStr);
	}
	if (properties.HasMember("animations") && properties["animations"].IsArray()) {
		const rapidjson::Value& animArray = properties["animations"];
		for (rapidjson::SizeType i = 0; i < animArray.Size(); ++i) {
			const rapidjson::Value& animPair = animArray[i];
			if (animPair.IsArray() && animPair.Size() == 2 &&
				animPair[0].IsString() && animPair[1].IsString()) {
				std::string shortName = animPair[0].GetString();
				std::string fileName = animPair[1].GetString();
				std::wstring wFileName(fileName.begin(), fileName.end());
				const Animation* anim = assetManager->LoadAnimation(wFileName);
				if (anim) {
					mAnimations[shortName] = anim;
				}
			}
		}
	}
}

bool Character::SetAnim(const std::string& animName)
{
	if (mAnimations.find(animName) != mAnimations.end())
	{
		mCurrentAnim = mAnimations[animName];
		mCurrentTime = 0.0f;
		return true;
	}
	return false;
}

void Character::UpdateAnim(float deltaTime)
{
	if (mCurrentAnim == nullptr)
	{
		if (!SetAnim("run"))
		{
			return;
		}
	}
	mCurrentTime += deltaTime;
	if (mCurrentAnim)
	{
		float animLength = mCurrentAnim->GetLength();
		if (animLength > 0.0f)
		{
			mCurrentTime = fmod(mCurrentTime, animLength);
			if (mCurrentTime < 0.0f)
				mCurrentTime += animLength;
		}
	}
	AnimationJob* pJob = new AnimationJob(this);
	JobManager::Get()->AddJob(pJob);
}

void Character::Update(float deltaTime)
{
	if (!mCurrentAnim)
	{
		SetAnim("run");
	}
	UpdateAnim(deltaTime);
}