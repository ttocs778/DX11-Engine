#include "stdafx.h"
#include "Physics.h"
#include "iostream"
#include "CollisionBox.h"
Physics::Physics()
{

}

Physics::AABB::AABB()
{

}

Physics::AABB::~AABB()
{

}

Physics::AABB::AABB(Vector3 minC, Vector3 maxC)
{
	minCorner = minC;
	maxCorner = maxC;
}

Physics::LineSegment::LineSegment(Vector3 sP, Vector3 eP)
{
	startingPoint = sP;
	endingPoint = eP;
}

bool Physics::Intersect(const AABB& a, const AABB& b, AABB* pOverlap)
{
	// Compute overlap along each axis.
	float overlapMinX = Math::Max(a.minCorner.x, b.minCorner.x);
	float overlapMaxX = Math::Min(a.maxCorner.x, b.maxCorner.x);
	float overlapMinY = Math::Max(a.minCorner.y, b.minCorner.y);
	float overlapMaxY = Math::Min(a.maxCorner.y, b.maxCorner.y);
	float overlapMinZ = Math::Max(a.minCorner.z, b.minCorner.z);
	float overlapMaxZ = Math::Min(a.maxCorner.z, b.maxCorner.z);

	// Check if there is a gap along any axis.
	if ((overlapMinX > overlapMaxX) && (!Math::IsCloseEnuf(overlapMinX, overlapMaxX)))
		return false;
	if ((overlapMinY > overlapMaxY) && (!Math::IsCloseEnuf(overlapMinY, overlapMaxY)))
		return false;
	if ((overlapMinZ > overlapMaxZ) && (!Math::IsCloseEnuf(overlapMinZ, overlapMaxZ)))
		return false;

	if (pOverlap)
	{
		pOverlap->minCorner = Vector3(overlapMinX, overlapMinY, overlapMinZ);
		pOverlap->maxCorner = Vector3(overlapMaxX, overlapMaxY, overlapMaxZ);
	}

	return true;
}

bool Physics::Intersect(const LineSegment& segment, const AABB& box,
	Vector3* pHitPoint)
{
	Vector3 d = segment.endingPoint - segment.startingPoint;
	float segLength = d.Length();
	if (Math::IsCloseEnuf(segLength, 0.0f))
		return false;

	d.Normalize();
	Vector3 p = segment.startingPoint;

	float tmin = 0.0f;
	float tmax = segLength;

	// ----------------- X Axis ------------------
	if (Math::IsCloseEnuf(d.x, 0.0f))
	{
		if (p.x < box.minCorner.x || p.x > box.maxCorner.x)
			return false;
	}
	else
	{
		float ood = 1.0f / d.x;
		float t1 = (box.minCorner.x - p.x) * ood;
		float t2 = (box.maxCorner.x - p.x) * ood;
		if (t1 > t2) {
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		if (tmin > tmax)
			return false;
	}

	// ----------------- Y Axis ------------------
	if (Math::IsCloseEnuf(d.y, 0.0f))
	{
		if (p.y < box.minCorner.y || p.y > box.maxCorner.y)
			return false;
	}
	else
	{
		float ood = 1.0f / d.y;
		float t1 = (box.minCorner.y - p.y) * ood;
		float t2 = (box.maxCorner.y - p.y) * ood;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		if (tmin > tmax)
			return false;
	}

	// ----------------- Z Axis ------------------
	if (Math::IsCloseEnuf(d.z, 0.0f))
	{
		if (p.z < box.minCorner.z || p.z > box.maxCorner.z)
			return false;
	}
	else
	{
		float ood = 1.0f / d.z;
		float t1 = (box.minCorner.z - p.z) * ood;
		float t2 = (box.maxCorner.z - p.z) * ood;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		if (tmin > tmax)
			return false;
	}

	if (pHitPoint)
	{
		*pHitPoint = p + d * tmin;
	}
	return true;
}

bool Physics::UnitTest()
{
	/*-----------------Intersect AABB test------------------*/
	struct TestAABB
	{
		AABB a;
		AABB b;
		AABB overlap;
	};
	const TestAABB testAABB[] =
	{
		{
		AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f)),
		AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-110.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f)),
		AABB(Vector3(-100.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(110.0f, 10.0f, 10.0f)),
		AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(100.0f, 10.0f, 10.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -110.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f)),
		AABB(Vector3(-10.0f, -100.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 110.0f, 10.0f)),
		AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 100.0f, 10.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -10.0f, -110.0f), Vector3(10.0f, 10.0f, -90.0f)),
		AABB(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, -90.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 110.0f)),
		AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 100.0f))
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-120.0f, -10.0f, -10.0f), Vector3(-110.0f, 10.0f, 10.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(110.0f, -10.0f, -10.0f), Vector3(120.0f, 10.0f, 10.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -120.0f, -10.0f), Vector3(10.0f, -110.0f, 10.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, 110.0f, -10.0f), Vector3(10.0f, 120.0f, 10.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -10.0f, -120.0f), Vector3(10.0f, 10.0f, -110.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		AABB(Vector3(-10.0f, -10.0f, 110.0f), Vector3(10.0f, 10.0f, 120.0f)),
		AABB(Vector3::One, Vector3::Zero)
		},
	};
	bool allPassed = true;
	const int numTests = sizeof(testAABB) / sizeof(TestAABB);

	for (int i = 0; i < numTests; ++i)
	{
		AABB computedOverlap;
		bool intersect = Intersect(testAABB[i].a, testAABB[i].b, &computedOverlap);

		bool expectsIntersection = true;
		if (Vector3::IsCloseEnuf(testAABB[i].overlap.minCorner, Vector3::One) &&
			Vector3::IsCloseEnuf(testAABB[i].overlap.maxCorner, Vector3::Zero))
		{
			expectsIntersection = false;
		}

		if (expectsIntersection)
		{
			if (!intersect)
			{
				std::cout << "Test " << i << " failed: expected an intersection but none was detected." << std::endl;
				allPassed = false;
			}
			else
			{
				if (!Vector3::IsCloseEnuf(computedOverlap.minCorner, testAABB[i].overlap.minCorner) ||
					!Vector3::IsCloseEnuf(computedOverlap.maxCorner, testAABB[i].overlap.maxCorner))
				{
					allPassed = false;
				}
			}
		}
		else
		{
			if (intersect)
			{
				std::cout << "Test " << i << " failed: expected no intersection but one was detected." << std::endl;
				allPassed = false;
			}
		}
	}
	/*-----------------Segment AABB test------------------*/
	struct TestSegment
	{
		AABB box;
		LineSegment segment;
		bool hit;
		Vector3 point;
	};
	const TestSegment testSegment[] =
	{
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(-110.0f, 0.0f, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)),
		true, Vector3(-100.0f, 0.0f, 0.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, -110.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f)),
		true, Vector3(0.0f, -100.0f, 0.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 0.0f, -110.0f), Vector3(0.0f, 0.0f, -90.0f)),
		true, Vector3(0.0f, 0.0f, -100.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(110.0f, 0.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f)),
		true, Vector3(100.0f, 0.0f, 0.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 110.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f)),
		true, Vector3(0.0f, 100.0f, 0.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 0.0f, 110.0f), Vector3(0.0f, 0.0f, 90.0f)),
		true, Vector3(0.0f, 0.0f, 100.0f)
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(-120.0f, 0.0f, 0.0f), Vector3(-110.0f, 0.0f, 0.0f)),
		false, Vector3::Zero
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, -120.0f, 0.0f), Vector3(0.0f, -110.0f, 0.0f)),
		false, Vector3::Zero
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, -110.0f)),
		false, Vector3::Zero
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(120.0f, 0.0f, 0.0f), Vector3(110.0f, 0.0f, 0.0f)),
		false, Vector3::Zero
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 120.0f, 0.0f), Vector3(0.0f, 110.0f, 0.0f)),
		false, Vector3::Zero
		},
		{
		AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		LineSegment(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 110.0f)),
		false, Vector3::Zero
		},
	};
	const int numSegTests = sizeof(testSegment) / sizeof(TestSegment);
	for (int i = 0; i < numSegTests; ++i)
	{
		Vector3 computedHit = Vector3::Zero;
		bool hit = Intersect(testSegment[i].segment, testSegment[i].box, &computedHit);

		if (testSegment[i].hit)
		{
			if (!hit)
			{
				allPassed = false;
			}
			else if (!Vector3::IsCloseEnuf(computedHit, testSegment[i].point))
			{
				allPassed = false;
			}
		}
		else
		{
			if (hit)
			{
				allPassed = false;
			}
		}
	}
	return allPassed;
}

void Physics::AddObj(const CollisionBox* pObj)
{
	mCollisionBox.push_back(pObj);
}

void Physics::RemoveObj(const CollisionBox* pObj)
{
	auto remove = std::find(mCollisionBox.begin(), mCollisionBox.end(), pObj);
	if (remove != mCollisionBox.end())
	{
		mCollisionBox.erase(remove);
	}
}

bool Physics::RayCast(const LineSegment& segment, Vector3*
	pHitPoint)
{
	bool found = false;
	Vector3 bestHit = Vector3::Zero;
	float bestDist = FLT_MAX;

	for (size_t i = 0; i < mCollisionBox.size(); ++i)
	{
		AABB box = mCollisionBox[i]->GetAABB();
		Vector3 hit;
		if (Intersect(segment, box, &hit))
		{
			float dist = (hit - segment.startingPoint).Length();
			if (dist < bestDist)
			{
				bestDist = dist;
				bestHit = hit;
				found = true;
			}
		}
	}

	if (found && pHitPoint)
	{
		*pHitPoint = bestHit;
	}

	return found;
}