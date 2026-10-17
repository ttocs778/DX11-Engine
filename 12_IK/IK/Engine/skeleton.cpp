#include "stdafx.h"
#include "skeleton.h"
#include "jsonUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>

int Skeleton::GetBoneIndex(const std::string& boneName) const
{
    int numBones = (int)mBones.size();
    for (int i = 0; i < numBones; ++i)
    {
        if (mBones[i].mName == boneName)
            return (int)i;
    }
    return -1;
}

/*static*/ Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
    Skeleton* pSkeleton = new Skeleton();
	if (false == pSkeleton->Load(fileName))
	{
		delete pSkeleton;
		return nullptr;
	}
	return pSkeleton;
}

bool Skeleton::Load(const WCHAR* fileName)
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
        str != "itpskel" ||
        ver != 1)
        return false;

    const rapidjson::Value& bonecount = doc["bonecount"];
    if (!bonecount.IsUint())
        return false;

    size_t count = bonecount.GetUint();
    mBones.reserve(count);

    const rapidjson::Value& bones = doc["bones"];
    if (!bones.IsArray())
        return false;

    if (bones.Size() != count)
        return false;

    Bone temp;
    for (rapidjson::SizeType i = 0; i < count; i++)
    {
        if (!bones[i].IsObject())
            return false;

        const rapidjson::Value& name = bones[i]["name"];
        temp.mName = name.GetString();

        const rapidjson::Value& parent = bones[i]["parent"];
        temp.mParent = parent.GetInt();

        const rapidjson::Value& bindpose = bones[i]["bindpose"];
        if (!bindpose.IsObject())
            return false;
        if (false == GetQuaternionFromJSON(bindpose, "rot", temp.mBindPose.mRot))
            return false;
        if (false == GetVectorFromJSON(bindpose, "trans", temp.mBindPose.mPos))
            return false;

        mBones.push_back(temp);
    }

    // Now that we have the bones
    ComputeGlobalInvBindPose();

    return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
    mInvBindMat.resize(mBones.size());

    // Loop through all the bones calculating the Bind Pose of each
    for (int i = 0; i < mBones.size(); ++i)
    {
        mInvBindMat[i] = mBones[i].mBindPose.ToMatrix();
        if (mBones[i].mParent >= 0)
            mInvBindMat[i] = mInvBindMat[i] * mInvBindMat[mBones[i].mParent];
    }

    // Loop through all the bones inverting the Bind Pose of each
    for (int i = 0; i < mBones.size(); ++i)
    {
        mInvBindMat[i].Invert();
    }
}
