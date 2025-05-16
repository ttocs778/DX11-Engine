#pragma once

#include "Component.h"               
#include "Game.h"                  
#include "jsonUtil.h"
#include "SkinnedObj.h"
#include "Skeleton.h"

class Character : public Component
{
private:
	SkinnedObj* mSkinnedObj;
	Skeleton* mSkeleton;
	std::unordered_map<std::string, const Animation*> mAnimations;
	const Animation* mCurrentAnim;
	Game* mGame = nullptr;
	float mCurrentTime;
public:
	Character(RenderObj* pObj, Game* pGame);
	~Character();
	void LoadProperties(const rapidjson::Value& properties) override;
	bool SetAnim(const std::string& animName);
	void UpdateAnim(float deltaTime);
	void Update(float deltaTime) override;
};