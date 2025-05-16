#pragma once
#include "engineMath.h"
struct PerObjectConstants {
	Matrix4 c_modelToWorld;
};

class VertexBuffer;
class RenderObj
{
public:
	RenderObj(const VertexBuffer* vertexBuffer);
	virtual ~RenderObj();
	virtual void Draw();
	PerObjectConstants& GetObjectConstants() {
		return mObjectConstants;
	}
	void SetObjectConstants(PerObjectConstants objectConstants) {
		mObjectConstants = objectConstants;
	}

protected:
	const VertexBuffer* mBuffer;
	PerObjectConstants mObjectConstants;
	ID3D11Buffer* mObjectBuffer;
};

