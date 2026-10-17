#pragma once
#include "Component.h"
#include "boneTransform.h"
#include <functional>

class Animation;
class Skeleton;
class SkinnedObj;

class Character : public Component
{
public:
    typedef std::function<void(std::vector<BoneTransform>&)> CustomAnimFcn;
    Character(SkinnedObj* renderObj);
    ~Character();

    void LoadProperties(const rapidjson::Value& properties) override;
    void Update(float deltaTime) override;

    bool SetAnim(const std::string& animName);
    Skeleton* GetSkeleton() { return mSkeleton; }
    void CustomAnim(CustomAnimFcn customAnim) { mCustomAnim = customAnim; }

protected:
    void UpdateAnim(float deltaTime);

    SkinnedObj* mSkinObj;
    Skeleton* mSkeleton;
    std::unordered_map<std::string, const Animation*> mAnims;
    const Animation* mCurAnim;
    float mAnimTime;
    CustomAnimFcn mCustomAnim;
};