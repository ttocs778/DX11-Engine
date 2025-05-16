#pragma once
#include "engineMath.h"

class CollisionBox;
class Physics
{
public:
	Physics();
	class AABB
	{
	public:
		Vector3 minCorner;
		Vector3 maxCorner;
		AABB();
		~AABB();
		AABB(Vector3 minC, Vector3 maxC);
	};

	class LineSegment
	{
	public:
		Vector3 startingPoint;
		Vector3 endingPoint;
		LineSegment(Vector3 sP, Vector3 eP);
	};

	static bool Intersect(const AABB& a, const AABB& b, AABB* pOverlap = nullptr);
	static bool Intersect(const LineSegment& segment, const AABB& box,
		Vector3* pHitPoint = nullptr);
	static bool UnitTest();
	std::vector<const CollisionBox*> mCollisionBox;
	void AddObj(const CollisionBox* pObj);
	void RemoveObj(const CollisionBox* pObj);
	bool RayCast(const LineSegment& segment, Vector3*
		pHitPoint = nullptr);
};



