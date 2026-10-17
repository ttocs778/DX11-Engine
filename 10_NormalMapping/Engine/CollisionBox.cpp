#include "stdafx.h"
#include "CollisionBox.h"
#include "RenderObj.h"
#include "../Lab08_Collisions/Source/game.h"
CollisionBox::CollisionBox(RenderObj* pObj, Physics* pPhysics) : Component(pObj), mPhysics(pPhysics)
{
	mPhysics->AddObj(this);

}

CollisionBox::~CollisionBox()
{
	mPhysics->RemoveObj(this);
}
void CollisionBox::LoadProperties(const rapidjson::Value& properties)
{
	Vector3 min;
	Vector3 max;
	if (GetVectorFromJSON(properties, "min", min) && GetVectorFromJSON(properties, "max", max))
	{
		mBox = Physics::AABB(min, max);
	}
}

Physics::AABB CollisionBox::GetAABB() const
{
	Physics::AABB newBox = mBox;
	Vector3 scale = mOwner->GetObjectConstants().c_modelToWorld.GetScale();
	Vector3 translation = mOwner->GetObjectConstants().c_modelToWorld.GetTranslation();
	newBox.minCorner = (newBox.minCorner * scale.x) + translation;
	newBox.maxCorner = (newBox.maxCorner * scale.x) + translation;
	return newBox;
}