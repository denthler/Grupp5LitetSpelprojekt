class AABB
{
	float center[3];
	float halfWidth;
	float halfHeight;
	float halfDepth;
public:
	AABB(float * center, float width, float height, float depth);

	bool AABBvsAABB(AABB box);
};