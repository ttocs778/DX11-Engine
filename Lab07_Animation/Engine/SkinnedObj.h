#pragma once
#include "RenderObj.h"
const int MAX_SKELETON_BONES = 80;
class SkinnedObj : public RenderObj
{
	struct SkinConstants
	{
		Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
	};
public:
	SkinnedObj(const Mesh* mesh);
	~SkinnedObj();
	SkinConstants skinConstants;
	void Draw() override;
private:
	ID3D11Buffer* mSkinBuffer;
};

