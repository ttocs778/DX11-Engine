#pragma once

#include "boneTransform.h"
class AssetManager;

class Skeleton
{
public:
    class Bone
    {
    public:
        BoneTransform mBindPose;
        std::string mName;
        int mParent;
    };

    size_t GetNumBones() const { return mBones.size(); }
    const Bone& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<Bone>& GetBones() const { return mBones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mInvBindMat; }
    int GetBoneIndex(const std::string& boneName) const;

    static Skeleton* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
    bool Load(const WCHAR* fileName);

private:
    void ComputeGlobalInvBindPose();

    std::vector<Bone> mBones;
    std::vector<Matrix4> mInvBindMat;
};