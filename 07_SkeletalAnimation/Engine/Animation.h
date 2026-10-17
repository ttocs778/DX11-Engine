#pragma once
#include "BoneTransform.h"
#include "Skeleton.h"
class AssetManager;
class Animation
{
private:
	uint32_t mBonesNum;
	uint32_t mFramesNum;
	float mAnimationLength;
	std::vector<std::vector<BoneTransform>> mAnimationData;
public:
	Animation();
	uint32_t GetNumBones() const;
	uint32_t GetNumFrames() const;
	float GetLength() const;
	static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
	bool Load(const WCHAR* fileName);
	void GetGlobalPoseAtTime(
		std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime
	) const;
};

