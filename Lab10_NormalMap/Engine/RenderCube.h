#pragma once
#include "RenderObj.h"
class RenderCube : public RenderObj
{
public:
	RenderCube(Material* material);
	~RenderCube();
private:
	VertexBuffer* mCubeBuffer = nullptr;
};




