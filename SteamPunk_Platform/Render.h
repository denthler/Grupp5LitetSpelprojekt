#ifndef _RENDER_H_
#define _RENDER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>
#include <fstream>
#include "d3dx11Effect.h"
#include "PointLight.h"
#include "model.h"

using namespace std;


class Render
{

private:
	struct MatrixBufferType
	{
		D3DMATRIX world;
		D3DMATRIX view;
		D3DMATRIX projection;
	};
	
	struct MaterialType
	{
		D3DXVECTOR4 difColor;
		D3DXVECTOR4 ambColor;
		D3DXVECTOR3 padding;
		bool hasTexture;
	};
	
	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 lightPosition;
		D3DXVECTOR3 lightAtt;
		float range;
		D3DXVECTOR4 ambientColor;
	};


public:
	Render();
	Render(const Render&);
	~Render();

	bool Initialize(ID3D11Device*, HWND, WCHAR*, D3DXMATRIX);
	void Shutdown();
	bool UpdateRender(ID3D11DeviceContext*,
	D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, PointLightClass*, ModelClass::Material*);
	void Draw(ID3D11DeviceContext*, int);
	void CleanShader();
	void UpdateFrustum(D3DXMATRIX view, D3DXMATRIX proj);
	bool InsideFrustum(D3DXVECTOR3 min, D3DXVECTOR3 max);

private:
	
	ID3DX11EffectConstantBuffer* cbMatrix;
	ID3DX11EffectConstantBuffer* lightConstantBuffer;
	ID3DX11Effect* effect;
	ID3D11InputLayout* layout;
	ID3DX11EffectTechnique* eTech;
	ID3D11Buffer* cbMatrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11SamplerState* sampleState; 
	ID3DX11EffectSamplerVariable* samplerVariable; 
	ID3DX11EffectShaderResourceVariable* shaderResourceView;
	ID3D11Buffer* materialBuffer;
	ID3DX11EffectConstantBuffer* Mat;
	D3DXMATRIX projectionMatrix;
	//Frustum
	D3DXPLANE plane[6];
};

#endif 