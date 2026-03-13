#include "stdafx.h"
#include "Components\character.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "SkinnedObj.h"
#include "stringUtil.h"


Character::Character(SkinnedObj* renderObj)
    : Component(renderObj)
    , mSkinObj(renderObj)
    , mSkeleton(nullptr)
    , mCurAnim(nullptr)
    , mAnimTime(0.0f)
{
}

Character::~Character()
{
}

void Character::LoadProperties(const rapidjson::Value& properties)
{
    std::wstring skeleton;
    GetWStringFromJSON(properties, "skeleton", skeleton);
    mSkeleton = AssetManager::Get()->LoadSkeleton(skeleton);
    const rapidjson::Value& anims = properties["animations"];
    DbgAssert(anims.IsArray(), "Invalid Skeleton Format");
    for (rapidjson::SizeType i = 0; i < anims.Size(); ++i)
    {
        DbgAssert(anims[i].IsArray(), "Invalid Skeleton Format");
        std::string animName = anims[i][0].GetString();
        std::wstring animFile;
        StringUtil::String2WString(animFile, anims[i][1].GetString());
        mAnims[animName] = AssetManager::Get()->LoadAnimation(animFile);
    }
}

void Character::Update(float deltaTime)
{
    if (nullptr == mCurAnim)
    {
        SetAnim("idle");
    }
    UpdateAnim(deltaTime);
}

bool Character::SetAnim(const std::string& animName)
{
    if (mAnims.find(animName) != mAnims.end())
    {
        mCurAnim = mAnims[animName];
        mAnimTime = 0.0f;
        return true;
    }
    return false;
}

void Character::UpdateAnim(float deltaTime)
{
    if (mCurAnim)
    {
        // update the animation time
        mAnimTime += 1.0f * deltaTime;
        while (mAnimTime > mCurAnim->GetLength())
            mAnimTime -= mCurAnim->GetLength();

        // calculate the anim poses
        std::vector<BoneTransform> bones;
        mCurAnim->GetBoneTransforms(bones, mSkeleton, mAnimTime);

        if (mCustomAnim)
        {   // custom animation here
            mCustomAnim(bones);
        }

        // convert boneTransforms to model pose matrix
        std::vector<Matrix4> outPoses;
        Animation::BonesToGlobalPose(bones, outPoses, mSkeleton);

        // combine with inv bind boses
        const std::vector<Matrix4>& invBind = mSkeleton->GetGlobalInvBindPoses();
        for (size_t i = 0; i < mSkeleton->GetNumBones(); ++i)
        {
            mSkinObj->mSkinConst.c_skinMatrix[i] = invBind[i] * outPoses[i];
        }
    }
}

