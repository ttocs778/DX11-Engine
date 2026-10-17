#pragma once
#include "engineMath.h"

class Component;
class Mesh;

class RenderObj
{
public:
    struct PerObjectConstants
    {
        Matrix4 c_modelToWorld;
    };

    RenderObj(const Mesh* mesh);
    virtual ~RenderObj();

    void AddComponent(Component* pComp);
    // Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}

		return nullptr;
	}

    virtual void Draw() const;
    virtual void Update(float deltaTime);

    PerObjectConstants mConstants;

protected:
    const Mesh* mMesh;
    ID3D11Buffer* mConstBuffer;
    std::vector<Component*> mComponents;
};