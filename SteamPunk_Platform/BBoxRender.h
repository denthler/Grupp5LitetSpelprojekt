#include <vector>
#include "model.h"

class BBoxRender
{
	void BBoxToVertices(ModelClass::BoundingBox bbox);
	std::vector<float> vertices;

	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	ID3D11VertexShader * VS;
	ID3D11PixelShader * PS;
	ID3D11Buffer * buffer;
	ID3D11InputLayout * vertLayout;

public:
	BBoxRender(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	~BBoxRender();
	
	void Update(std::vector<ModelClass::BoundingBox> bboxes);
	void Draw();
};