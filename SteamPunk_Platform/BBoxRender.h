#include <vector>
#include "model.h"

class BBoxRender
{
	void BBoxToVertices(ModelClass::BoundingBox bbox);
	std::vector<float> vertices;
	std::vector<byte> indices;
public:
	BBoxRender() {};
	~BBoxRender();
	
	void Update();
	void Draw();
};