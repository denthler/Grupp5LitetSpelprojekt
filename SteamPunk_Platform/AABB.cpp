#include "AABB.h"
#include <cmath>

AABB::AABB(float center[], float width, float height, float depth)
{
	this->center[0] = center[0];
	this->center[1] = center[1];
	this->center[2] = center[2];
	this->halfWidth = width / 2.0f;
	this->halfHeight = height / 2.0f;
	this->halfDepth = depth / 2.0f;
}

// Returns true if this AABB is colliding or intersecting with box.
bool AABB::AABBvsAABB(AABB box)
{
	if (abs(this->center[0] - box.center[0]) > (this->halfWidth + box.halfWidth)) return false;
	if (abs(this->center[1] - box.center[1]) > (this->halfHeight + box.halfHeight)) return false;
	if (abs(this->center[2] - box.center[2]) > (this->halfDepth + box.halfDepth)) return false;
	return true;
}