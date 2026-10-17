#pragma once
#include "RenderObj.h"
class RenderCube : public RenderObj
{
public:
	RenderCube();
	~RenderCube();
private:
	VertexBuffer* mCubeBuffer;
};




