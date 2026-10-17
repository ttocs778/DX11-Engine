#include "stdafx.h"
#include "animation.h"
#include "jsonUtil.h"
#include "skeleton.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>

/*static*/ Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
    Animation* pAnim = new Animation();
	if (false == pAnim->Load(fileName))
	{
		delete pAnim;
		return nullptr;
	}
	return pAnim;
}

bool Animation::Load(const WCHAR* fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        return false;

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
        return false;

    std::string str = doc["metadata"]["type"].GetString();
    int ver = doc["metadata"]["version"].GetInt();
    // Check the metadata
    if (!doc["metadata"].IsObject() ||
        str != "itpanim" ||
        ver != 2)
        return false;

    const rapidjson::Value& sequence = doc["sequence"];
    if (!sequence.IsObject())
        return false;

    GetUintFromJSON(sequence, "frames", mNumFrames);
    GetFloatFromJSON(sequence, "length", mDuration);
    GetUintFromJSON(sequence, "bonecount", mNumBones);
    mAnimData.resize(mNumBones);
    const rapidjson::Value& tracks = sequence["tracks"];
    if (!tracks.IsArray())
        return false;
    for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
    {
        if (!tracks[i].IsObject())
            return false;
        int boneIndex = -1;
        GetIntFromJSON(tracks[i], "bone", boneIndex);
        const rapidjson::Value& transforms = tracks[i]["transforms"];
        if (!transforms.IsArray())
            return false;
        for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
        {
            if (!transforms[j].IsObject())
                return false;
            BoneTransform bone;
            GetQuaternionFromJSON(transforms[j], "rot", bone.mRot);
            GetVectorFromJSON(transforms[j], "trans", bone.mPos);
            mAnimData[boneIndex].push_back(bone);
        }
    }
    return true;
}

/// <summary>
/// Calculates the bone transforms for the animation at the specified time.
/// This will include interpolating keyframes.
/// </summary>
/// <param name="bones">This is the output... a vector of all the BoneTransforms for the animation at the specified time</param>
/// <param name="inSkeleton">The Skeleton is needed to describe the hierarchy</param>
/// <param name="inTime">The time in the animation - make sure this does not exceed the length of the animation</param>
void Animation::GetBoneTransforms(std::vector<BoneTransform>& bones, const Skeleton* inSkeleton, float inTime) const
{
    float timePerFrame = mDuration / (mNumFrames - 1);
    float frac = inTime / timePerFrame;
    int fromKey = (int)frac;
    int toKey = ceil(frac);
    float lerp = frac - fromKey;

    size_t numBones = inSkeleton->GetNumBones();
    bones.resize(numBones);
    for (size_t i = 0; i < numBones; ++i)
    {
        const Skeleton::Bone& bone = inSkeleton->GetBone(i);
        if (mAnimData[i].size() > 0)
        {
            bones[i] = BoneTransform::Interpolate(mAnimData[i][fromKey], mAnimData[i][toKey], lerp);
        }
        else
        {
            bones[i] = BoneTransform();
        }
    }
}

/// <summary>
/// Converts the vector of BoneTransforms into matrices.
/// Do this once after all the BoneTransforms are blended.
/// </summary>
/// <param name="bones">Input to the function... the vector of final BoneTransforms</param>
/// <param name="outPoses">Output of the function... the animation matrices. These do NOT include the inverse bind poses.</param>
/// <param name="inSkeleton">The Skeleton is needed to describe the hierarchy</param>
/*static*/ void Animation::BonesToGlobalPose(const std::vector<BoneTransform>& bones, std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton)
{
    size_t numBones = inSkeleton->GetNumBones();
    outPoses.resize(numBones);
    for (size_t i = 0; i < numBones; ++i)
    {
        const Skeleton::Bone& bone = inSkeleton->GetBone(i);
        outPoses[i] = bones[i].ToMatrix();
        if (bone.mParent >= 0)
            outPoses[i] = outPoses[i] * outPoses[bone.mParent];
    }
}