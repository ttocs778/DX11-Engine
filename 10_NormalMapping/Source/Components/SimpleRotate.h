#pragma once
#include "Component.h"
#include "RenderObj.h"
class SimpleRotate :
	public Component
{
public:
	SimpleRotate(RenderObj* owner) : Component(owner) {}


	void LoadProperties(const rapidjson::Value& props) override
	{
		GetFloatFromJSON(props, "speed", mSpeed);
	}

	void Update(float deltaTime) override
	{
		if (Math::IsCloseEnuf(mSpeed, 0.0f)) return;

		Matrix4 world = mOwner->GetObjectConstants().c_modelToWorld;

		float angle = mSpeed * deltaTime;
		Matrix4 incRot = Matrix4::CreateRotationZ(angle);

		world = world * incRot;

		mOwner->GetObjectConstants().c_modelToWorld = world;
	}

private:
	float mSpeed = 0.0f;
};

