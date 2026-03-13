#pragma once
#include "Component.h"
#include "boneTransform.h"

class Character;
class Game;
class Skeleton;

class IKChain : public Component
{
public:
    IKChain(RenderObj* renderObj, Game* game);
    ~IKChain();

    void LoadProperties(const rapidjson::Value& properties) override;

private:
    Game* mGame;
    Character* mCharacter;
    Skeleton* mSkeleton;
    RenderObj* mTargetObj;
    std::vector<std::string> mBoneNames;
    std::vector<int> mBoneIndices;

    // TODO You may want more functions or variables here

    void DoAnim(std::vector<BoneTransform>& bones);
};