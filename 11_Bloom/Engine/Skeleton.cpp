#include "stdafx.h"
#include "Skeleton.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "material.h"
#include "Shader.h"
#include "stringUtil.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
#include <fstream>
#include <sstream>

Skeleton::Skeleton()
{

}

size_t Skeleton::GetNumBones() const
{
	return mBones.size();
}

const Skeleton::Bone& Skeleton::GetBone(size_t idx) const
{
	return mBones[idx];
}
const std::vector<Skeleton::Bone>& Skeleton::GetBones() const
{
	return mBones;
}

const std::vector<Matrix4>& Skeleton::GetGlobalInvBindPoses() const
{
	return inverseBondPoseMatrices;
}
Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
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
		temp.boneName = name.GetString();
		const rapidjson::Value& parent = bones[i]["parent"];
		temp.parentIndex = parent.GetInt();
		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
			return false;
		if (false == GetQuaternionFromJSON(bindpose, "rot", temp.boneTransform.rotation))
			return false;
		if (false == GetVectorFromJSON(bindpose, "trans", temp.boneTransform.translation))
			return false;
		mBones.push_back(temp);
	}
	// Now that we have the bones
	ComputeGlobalInvBindPose();
	return true;
}
