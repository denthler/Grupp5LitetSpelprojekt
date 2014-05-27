#include <vector>
#include "model.h"

class BBoxRender
{
	void BBoxToVertices(ModelClass::BoundingBox bbox);
	std::vector<float> vertices;
	std::vector<D3DXMATRIX> matrices;

	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	ID3D11VertexShader * VS;
	ID3D11PixelShader * PS;
	ID3D11Buffer * buffer, * matrixBuffer;
	ID3D11InputLayout * vertLayout;

public:
	BBoxRender();
	~BBoxRender();
	
	void Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	void Update(std::vector<ModelClass::BoundingBox> & bboxes);
	void Draw(std::vector<D3DXMATRIX> & worldMatrices, D3DXMATRIX view, D3DXMATRIX proj);
};