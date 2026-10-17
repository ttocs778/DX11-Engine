#include "stdafx.h"
#include "Components\ikChain.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "SkinnedObj.h"
#include "stringUtil.h"
#include "Components\character.h"
#define IK_DEBUG_DRAW 0

IKChain::IKChain(RenderObj* renderObj, Game* game)
	: Component(renderObj)
	, mGame(game)
	, mCharacter(nullptr)
	, mSkeleton(nullptr)
	, mTargetObj(nullptr)
{
}

IKChain::~IKChain()
{
}

void IKChain::LoadProperties(const rapidjson::Value& properties)
{
	const rapidjson::Value& joints = properties["joints"];
	DbgAssert(joints.IsArray(), "Invalid IKChain Format");
	for (rapidjson::SizeType i = 0; i < joints.Size(); ++i)
	{
		std::string jointName = joints[i].GetString();
		mBoneNames.push_back(jointName);
	}
	std::string targetName;
	if (GetStringFromJSON(properties, "target", targetName))
	{
		mTargetObj = mGame->FindObjByName(targetName);
	}

	mCharacter = mOwner->GetComponent<Character>();
	if (mCharacter)
	{
		mSkeleton = mCharacter->GetSkeleton();
		if (mSkeleton)
		{
			for (size_t i = 0; i < mBoneNames.size(); ++i)
			{
				mBoneIndices.push_back(mSkeleton->GetBoneIndex(mBoneNames[i]));
			}
			mCharacter->CustomAnim([this](std::vector<BoneTransform>& bones)
				{
					DoAnim(bones);
				});
		}
	}

	// TODO You may want more initialization here
}

static void DrawAxis(const Matrix4& mat)
{
	Vector3 pos = mat.GetTranslation();
	Vector3 dir = mat.GetXAxis();
	Graphics::Get()->Debug_Line(pos, pos + 20.0f * dir, Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	dir = mat.GetYAxis();
	Graphics::Get()->Debug_Line(pos, pos + 20.0f * dir, Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f));
	dir = mat.GetZAxis();
	Graphics::Get()->Debug_Line(pos, pos + 20.0f * dir, Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f));
}

/// <summary>
/// Perform customized animation.
/// The animation has already calculated the bones. Since the vector is passed by reference, you may modify the bones.
/// </summary>
/// <param name="bones">in/out the vector of bone transforms</param>
void IKChain::DoAnim(std::vector<BoneTransform>& bones)
{
	if (nullptr == mTargetObj)
		return;

	// convert boneTransforms to model pose matrix
	std::vector<Matrix4> outPoses;
	Animation::BonesToGlobalPose(bones, outPoses, mSkeleton);
	// TODO YOUR CODE HERE
#if IK_DEBUG_DRAW
	Matrix4 charWorld = mOwner->mConstants.c_modelToWorld;
	DrawAxis(charWorld);
	Matrix4 targetWorld = mTargetObj->mConstants.c_modelToWorld;
	DrawAxis(targetWorld);
	Vector3 charPos = charWorld.GetTranslation();
	Vector3 targetPos = targetWorld.GetTranslation();
	Graphics::Get()->Debug_Line(
		charPos,
		targetPos,
		Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f)
	);
	for (int jointIdx : mBoneIndices)
	{
		Matrix4 jointWorld = charWorld * outPoses[jointIdx];
		DrawAxis(jointWorld);
	}
#endif
	std::vector<Vector3> origPos;
	origPos.reserve(mBoneIndices.size());
	for (int boneIdx : mBoneIndices)
	{
		origPos.push_back(outPoses[boneIdx].GetTranslation());
	}

	int n = (int)origPos.size();
	if (n < 2)
		return;

	std::vector<float> linkLen(n - 1);
	for (int i = 0; i < n - 1; ++i)
		linkLen[i] = (origPos[i + 1] - origPos[i]).Length();

	origPos.reserve(mBoneIndices.size());
	for (int boneIdx : mBoneIndices)
	{
		origPos.push_back(outPoses[boneIdx].GetTranslation());
	}

	if (n < 2) return;

	for (int i = 0; i < n - 1; ++i)
		linkLen[i] = (origPos[i + 1] - origPos[i]).Length();


	Matrix4 invCharWorld = mOwner->mConstants.c_modelToWorld;
	invCharWorld.Invert();

	Vector3 targetWorldTrans = mTargetObj->mConstants.c_modelToWorld.GetTranslation();

	Vector3 targetModel = Matrix4::Transform(targetWorldTrans, invCharWorld, /*w=*/1.0f);

	std::vector<Vector3> p = origPos;
	p[n - 1] = targetModel;
	for (int i = n - 2; i >= 0; --i)
	{
		Vector3 dir = p[i] - p[i + 1];
		float   r = dir.Length();
		if (r > 1e-5f)
			p[i] = p[i + 1] + dir * (linkLen[i] / r);
	}

	p[0] = origPos[0];
	for (int i = 0; i < n - 1; ++i)
	{
		Vector3 dir = p[i + 1] - p[i];
		float   r = dir.Length();
		if (r > 1e-5f)
			p[i + 1] = p[i] + dir * (linkLen[i] / r);
	}

	for (int i = 0; i < n - 1; ++i)
	{
		Graphics::Get()->Debug_Line(
			p[i],
			p[i + 1],
			Graphics::Color4(1.0f, 1.0f, 0.0f, 0.0f)
		);
	}

	int rootIdx = mBoneIndices[0];
	int rootParentIdx = mSkeleton->GetBone(rootIdx).mParent;
	Matrix4 parentModel = (rootParentIdx >= 0) ?
		outPoses[rootParentIdx] :
		Matrix4::Identity;

	for (int i = 0; i < n - 1; ++i)
	{
		int jointIdx = mBoneIndices[i];
		int childIdx = mBoneIndices[i + 1];

		const BoneTransform& J = mSkeleton->GetBone(jointIdx).mBindPose;
		const BoneTransform& C = mSkeleton->GetBone(childIdx).mBindPose;

		Matrix4 jointLocalBind =
			Matrix4::CreateTranslation(J.mPos) *
			Matrix4::CreateFromQuaternion(J.mRot);

		Matrix4 jointBindGlobal = jointLocalBind * parentModel;

		Vector3 origDir = C.mPos;
		origDir.Normalize();
		Vector3 modelDir = p[i + 1] - p[i];
		modelDir.Normalize();


		Matrix4 invJoint = jointBindGlobal; invJoint.Invert();
		Vector3 desiredDir = Matrix4::Transform(modelDir, invJoint, 0.0f);
		desiredDir.Normalize();

		float cosA = Math::Clamp(Vector3::Dot(origDir, desiredDir), -1.0f, 1.0f);
		Quaternion ikRot;
		if (Math::IsCloseEnuf(cosA, 1.0f))
		{
			ikRot = Quaternion::Identity;
		}
		else if (Math::IsCloseEnuf(cosA, -1.0f))
		{

			ikRot = Quaternion(Vector3::UnitY, Math::Pi);
		}
		else
		{
			Vector3 axis = Vector3::Cross(origDir, desiredDir);
			axis.Normalize();
			ikRot = Quaternion(axis, acosf(cosA));
		}

		Quaternion newRot = Quaternion::Concatenate(ikRot, J.mRot);
		newRot.Normalize();
		bones[jointIdx].mRot = newRot;

		parentModel =
			Matrix4::CreateTranslation(J.mPos) *
			Matrix4::CreateFromQuaternion(newRot) *
			parentModel;
	}
}