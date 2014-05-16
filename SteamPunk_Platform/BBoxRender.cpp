#include "BBoxRender.h"

BBoxRender::~BBoxRender()
{

}

void BBoxRender::Update()
{

}

void BBoxRender::Draw()
{

}

void BBoxRender::BBoxToVertices(ModelClass::BoundingBox bbox)
{
	float length = bbox.max.x - bbox.min.x;
	float height = bbox.max.y - bbox.min.y;
	float depth = bbox.max.z - bbox.min.z;

	// Data to draw a box with a single trianglestrip
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	// 4
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 3
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	// 7
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	// 8
	vertices.push_back(bbox.min.x + length);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 5
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 3
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	// 1
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);
	
	// 4
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 2
	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 7
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	// 6
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z + depth);

	// 5
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 2
	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 1
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);

	/* Data to draw a box with indexed trianglelist
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x + length);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z + depth);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	int offset = vertices.size();
	// First face
	indices.push_back(1 + offset);
	indices.push_back(2 + offset);
	indices.push_back(3 + offset);

	indices.push_back(2 + offset);
	indices.push_back(3 + offset);
	indices.push_back(8 + offset);

	// Second face
	indices.push_back(2 + offset);
	indices.push_back(7 + offset);
	indices.push_back(8 + offset);

	indices.push_back(7 + offset);
	indices.push_back(8 + offset);
	indices.push_back(5 + offset);

	// Third face
	indices.push_back(7 + offset);
	indices.push_back(4 + offset);
	indices.push_back(5 + offset);

	indices.push_back(4 + offset);
	indices.push_back(5 + offset);
	indices.push_back(6 + offset);

	// Fourth face
	indices.push_back(4 + offset);
	indices.push_back(1 + offset);
	indices.push_back(6 + offset);

	indices.push_back(1 + offset);
	indices.push_back(6 + offset);
	indices.push_back(3 + offset);

	// Fifth face
	indices.push_back(3 + offset);
	indices.push_back(8 + offset);
	indices.push_back(6 + offset);

	indices.push_back(8 + offset);
	indices.push_back(6 + offset);
	indices.push_back(5 + offset);

	// Sixth face
	indices.push_back(1 + offset);
	indices.push_back(2 + offset);
	indices.push_back(4 + offset);

	indices.push_back(2 + offset);
	indices.push_back(4 + offset);
	indices.push_back(7 + offset);
	*/
}