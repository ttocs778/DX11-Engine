#pragma once
#include "RenderObj.h"

class SkinnedObj : public RenderObj
{
public:
    static const int MAX_SKELETON_BONES = 80;
    struct SkinConstants
    {
        Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
    };

    SkinnedObj(const Mesh* mesh);
    virtual ~SkinnedObj();

    void Draw() const override;

    SkinConstants mSkinConst;

private:
    ID3D11Buffer* mSkinBuffer;
};