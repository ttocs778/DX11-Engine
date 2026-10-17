#pragma once
#include "Component.h"  
#include "../Engine/Physics.h"


class CollisionBox : Component
{
public:
	CollisionBox(RenderObj* pObj, Physics* pPhysics);
	~CollisionBox();
	void LoadProperties(const rapidjson::Value& properties) override;
	Physics::AABB GetAABB() const;
private:
	Physics::AABB mBox;
	Physics* mPhysics;
};