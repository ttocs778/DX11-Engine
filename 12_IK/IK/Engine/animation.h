#pragma once
#include "boneTransform.h"

class AssetManager;
class Skeleton;

class Animation
{
public:
    uint32_t GetNumBones() const { return mNumBones; }
	uint32_t GetNumFrames() const { return mNumFrames; }
	float GetLength() const { return mDuration; }

    static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
    bool Load(const WCHAR* fileName);

	void GetBoneTransforms(std::vector<BoneTransform>& bones, const Skeleton* inSkeleton, float inTime) const;
    static void BonesToGlobalPose(const std::vector<BoneTransform>& bones, std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton);

private:
    uint32_t mNumBones;
    uint32_t mNumFrames;
    float mDuration;
    std::vector<std::vector<BoneTransform>> mAnimData;
};