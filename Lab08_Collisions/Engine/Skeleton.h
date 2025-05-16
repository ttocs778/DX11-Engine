#pragma once
#include "BoneTransform.h"
class AssetManager;
class Skeleton
{
public:
	struct Bone
	{
		BoneTransform boneTransform;
		std::string boneName;
		int parentIndex;
	};

	Skeleton();
	size_t GetNumBones() const;
	const Bone& GetBone(size_t idx) const;
	const std::vector<Bone>& GetBones() const;
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const;
	static Skeleton* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);

	bool Load(const WCHAR* fileName);

private:
	std::vector<Bone> mBones;
	std::vector<Matrix4> inverseBondPoseMatrices;

	void ComputeGlobalInvBindPose()
	{
		inverseBondPoseMatrices.resize(mBones.size());
		for (int i = 0; i < mBones.size(); i++)
		{
			Matrix4 localBindPose = mBones[i].boneTransform.ToMatrix();
			if (mBones[i].parentIndex >= 0)
			{
				inverseBondPoseMatrices[i] =  localBindPose * inverseBondPoseMatrices[mBones[i].parentIndex];
			}
			else
			{
				inverseBondPoseMatrices[i] = localBindPose;
			}
		}
		for (auto& bindPoseInvMatrives : inverseBondPoseMatrices)
		{
			bindPoseInvMatrives.Invert();
		}
	}
};

