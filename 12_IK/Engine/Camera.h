#pragma once
#include "engineMath.h"


class Camera
{
public:
    struct PerCameraConstants
    {
        Matrix4 c_viewProj;
        Vector3 c_cameraPosition;
        float pad;
    };

    Camera();
    virtual ~Camera();
    void SetActive();

    void SetViewMat(const Matrix4& mat) { mWorld2Camera = mat; }
    const Matrix4& GetViewMat() const { return mWorld2Camera; }

protected:
    PerCameraConstants mConstants;
    ID3D11Buffer* mConstBuffer;
    Matrix4 mWorld2Camera;
    Matrix4 mProjMatrix;
};