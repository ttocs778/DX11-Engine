#pragma once
#include "engineMath.h"
class BoneTransform
{
public:
	Quaternion rotation;
	Vector3 translation;
	Matrix4 ToMatrix() const
	{
		Matrix4 rotationMatrix = Matrix4::CreateFromQuaternion(rotation);
		Matrix4 translationMatrix = Matrix4::CreateTranslation(translation);
		return rotationMatrix * translationMatrix;
	}
	static BoneTransform Interpolate(
		const BoneTransform& a, const BoneTransform& b, float f)
	{
		BoneTransform result;
		result.translation = Vector3::Lerp(a.translation, b.translation, f);
		result.rotation = Quaternion::Slerp(a.rotation, b.rotation, f);
		return result;
	}
};

