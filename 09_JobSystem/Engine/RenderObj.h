#pragma once
#include "Graphics.h"
#include "engineMath.h"
#include "mesh.h"
#include "Component.h"
struct PerObjectConstants
{
	Matrix4 c_modelToWorld;
};

class Material;
class VertexBuffer;
class RenderObj
{
public:
	RenderObj(const Mesh* mesh);
	virtual ~RenderObj();
	virtual void Draw();
	PerObjectConstants& GetObjectConstants()
	{
		return mObjectConstants;
	}
	void SetObjectConstants(PerObjectConstants objectConstants)
	{
		mObjectConstants = objectConstants;
	}
	void AddComponent(Component* pComp);
	virtual void Update(float deltaTime);

protected:
	const Mesh* mMesh;
	PerObjectConstants mObjectConstants;
	ID3D11Buffer* mObjectBuffer;
	std::vector<Component*> mComponents;
};

