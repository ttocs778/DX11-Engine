#include "stdafx.h"
#include "Animation.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "material.h"
#include "Shader.h"
#include "stringUtil.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
#include <fstream>
#include <sstream>
#include <BoneTransform.h>

Animation::Animation() :
	mBonesNum(0),
	mAnimationLength(0.0f),
	mFramesNum(0)
{

}

uint32_t Animation::GetNumBones() const
{
	return mBonesNum;
}

uint32_t Animation::GetNumFrames() const
{
	return mFramesNum;
}

float Animation::GetLength() const
{
	return mAnimationLength;
}


Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Animation* pAnimation = new Animation();
	if (false == pAnimation->Load(fileName))
	{
		delete pAnimation;
		return nullptr;
	}
	return pAnimation;
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
	mFramesNum = doc["sequence"]["frames"].GetUint();
	mAnimationLength = static_cast<float>(doc["sequence"]["length"].GetDouble());
	mBonesNum = doc["sequence"]["bonecount"].GetUint();
	mAnimationData.clear();
	const rapidjson::Value& tracks = doc["sequence"]["tracks"];
	if (!tracks.IsArray())
		return false;
	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		const rapidjson::Value& track = tracks[i];
		if (!track.IsObject())
			return false;
		int boneIndex = track["bone"].GetInt();
		const rapidjson::Value& transforms = track["transforms"];
		if (!transforms.IsArray())
			return false;
		std::vector<BoneTransform> trackTransforms;
		trackTransforms.reserve(transforms.Size());
		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& transform = transforms[j];
			if (!transform.IsObject())
				return false;
			BoneTransform bt;
			if (false == GetQuaternionFromJSON(transform, "rot", bt.rotation))
				return false;
			if (false == GetVectorFromJSON(transform, "trans", bt.translation))
				return false;
			trackTransforms.push_back(bt);
		}
		mAnimationData.push_back(trackTransforms);
	}

	return true;
}

void Animation::GetGlobalPoseAtTime(
	std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime
) const
{
	auto& bones = inSkeleton->GetBones();

	outPoses.resize(bones.size());

	if (mFramesNum < 1) {
		for (size_t i = 0; i < bones.size(); ++i) {
			outPoses[i] = Matrix4::Identity;
		}
		return;
	}
	float timePerFrame = mAnimationLength / static_cast<float>(mFramesNum - 1);
	// Determine the two key frame indices.
	int frame0 = static_cast<int>(floor(inTime / timePerFrame));
	int frame1 = frame0 + 1;
	float frac = (inTime - frame0 * timePerFrame) / timePerFrame;

	if (frame1 >= static_cast<int>(mFramesNum)) {
		frame1 = 0;
		float remainingTime = mAnimationLength - frame0 * timePerFrame;
		if (remainingTime > 0.0f) {
			frac = (inTime - frame0 * timePerFrame) / remainingTime;
		}
		else {
			frac = 0.0f;
		}
	}

	for (size_t i = 0; i < bones.size(); ++i) {
		Matrix4 localMatrix = Matrix4::Identity;

		if (i < mAnimationData.size() && !mAnimationData[i].empty()) {
			const std::vector<BoneTransform>& keyFrames = mAnimationData[i];

			int k0 = frame0;
			int k1 = frame1;
			if (k0 >= static_cast<int>(keyFrames.size()))
				k0 = static_cast<int>(keyFrames.size()) - 1;
			if (k1 >= static_cast<int>(keyFrames.size()))
				k1 = static_cast<int>(keyFrames.size()) - 1;

			BoneTransform interpBT = BoneTransform::Interpolate(keyFrames[k0], keyFrames[k1], frac);
			localMatrix = interpBT.ToMatrix();
		}
		else {
			localMatrix = Matrix4::Identity;
		}

		int parentIndex = bones[i].parentIndex;
		if (parentIndex >= 0) {
			outPoses[i] = localMatrix * outPoses[parentIndex];
		}
		else {
			outPoses[i] = localMatrix;
		}
	}
}