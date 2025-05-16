#include "stdafx.h"
#include "AnimationJob.h"
#include "../Source/Components/Character.h"
#include "BoneTransform.h"
#include <vector>
#include "Profiler.h"

AnimationJob::AnimationJob(Character* pCharacter)
	: mCharacter(pCharacter)
{


}

void AnimationJob::DoIt()
{
	PROFILE_SCOPE(AnimationJob);
	if (!mCharacter->mCurrentAnim)
		return;

	std::vector<Matrix4> globalPoses;
	mCharacter->mCurrentAnim->GetGlobalPoseAtTime(
		globalPoses,
		mCharacter->mSkeleton,
		mCharacter->mCurrentTime
	);

	const std::vector<Matrix4>& invBindPoses = mCharacter->mSkeleton->GetGlobalInvBindPoses();

	std::vector<Matrix4> finalSkinMatrices;
	finalSkinMatrices.resize(globalPoses.size());
	for (size_t i = 0; i < globalPoses.size(); ++i) {
		finalSkinMatrices[i] = invBindPoses[i] * globalPoses[i];
	}

	size_t count = finalSkinMatrices.size();
	if (count > MAX_SKELETON_BONES)
		count = MAX_SKELETON_BONES;

	for (size_t i = 0; i < count; ++i) {
		mCharacter->mSkinnedObj->skinConstants.c_skinMatrix[i] = finalSkinMatrices[i];
	}
}
