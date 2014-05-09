#include "Texture.h"

class Button
{
	struct AABB
	{
		float min[2];
		float max[2];

		bool Intersect(AABB box)
		{
			if (max[0] < box.min[0] || min[0] > box.max[0]) return false;
			if (max[1] < box.min[1] || min[1] > box.max[1]) return false;
			return true;
		}

		bool InsideBox(float x, float y)
		{
			if (min[0] > x || max[0] < x) return false;
			if (min[1] > y || max[1] < y) return false;
			return true;
		}
	};
protected:
	bool focused;
	TextureClass texture;
	AABB box;
public:
	Button() { focused = false; }
	~Button() { texture.Shutdown(); }
	bool Update(float x, float y) { focused = box.InsideBox(x, y);  return focused; }
	void Draw() {};
};