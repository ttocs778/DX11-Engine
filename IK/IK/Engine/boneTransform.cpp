#include "stdafx.h"
#include "boneTransform.h"

Matrix4 BoneTransform::ToMatrix() const
{
    return Matrix4::CreateFromQuaternion(mRot) * Matrix4::CreateTranslation(mPos);
}

/*static*/ BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
    BoneTransform ret;
    ret.mRot = Quaternion::Slerp(a.mRot, b.mRot, f);
    ret.mPos = Vector3::Lerp(a.mPos, b.mPos, f);
    return ret;
}
