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
#include "DirectionalLight.h"
#include "model.h"

using namespace std;


class Render
{

private:
	struct AnimationMatrixBufferType
	{
		D3DMATRIX boneTransform[30];
	};

	struct MatrixBufferType
	{
		D3DMATRIX world;
		D3DMATRIX view;
		D3DMATRIX projection;
		D3DMATRIX viewShadow;
		D3DMATRIX projectionShadow;
	};
	
	struct MaterialType
	{
		D3DXVECTOR4 difColor;
		D3DXVECTOR4 ambColor;
		int hasTexture;
		int hasNormal;
		int pad[2];
	};
	
	/*struct LightBufferType		/// PointLight
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 lightPosition;
		D3DXVECTOR3 lightAtt;
		float range;
		D3DXVECTOR4 ambientColor;
	};*/

	struct LightBufferType		/// DirectionalLight
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float pad1;
		D3DXVECTOR4 ambientColor;
	};

public:
	Render();
	Render(const Render&);
	~Render();

	bool Initialize(ID3D11Device*, HWND, WCHAR*, D3DXMATRIX);
	void Shutdown();
	bool UpdateRender(ID3D11DeviceContext*,
		D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ModelClass::Material, std::vector<D3DMATRIX>);
	bool UpdateRenderShadow(ID3D11DeviceContext*, D3DXMATRIX, std::vector<D3DMATRIX>);
	void Draw(ID3D11DeviceContext*, int, int);
	void CleanShader();
	void UpdateFrustum(D3DXMATRIX view, D3DXMATRIX proj);
	bool InsideFrustum(D3DXVECTOR3 min, D3DXVECTOR3 max);
	void setShadowMap(ID3D11ShaderResourceView* sM){ shadowMap = sM; };
	void setLightPosition(D3DXVECTOR3 playerPos);
	void SetProjectionMatrix(D3DXMATRIX proj)
	{
		projectionMatrix = proj;
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	}

private:	
	ID3DX11EffectConstantBuffer* cbMatrix;
	ID3DX11EffectConstantBuffer* cbAniMatrix;
	ID3DX11EffectConstantBuffer* lightConstantBuffer;
	ID3DX11Effect* effect;
	ID3D11InputLayout* layout;
	ID3DX11EffectTechnique* eTech;
	ID3D11Buffer* cbAniMatrixBuffer;
	ID3D11Buffer* cbMatrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow; 
	ID3DX11EffectSamplerVariable* samplerVariable;
	ID3DX11EffectSamplerVariable* samplerVariableShadow;
	ID3DX11EffectShaderResourceVariable* shaderResourceView;
	ID3DX11EffectShaderResourceVariable* normalMapShaderResourceView;
	ID3D11Buffer* materialBuffer;
	ID3DX11EffectConstantBuffer* Mat;
	D3DXMATRIX projectionMatrix;
	//Frustum
	D3DXPLANE plane[6];

	//ShadowMap
	D3DXMATRIX viewMatrixShadow;
	D3DXMATRIX projectionMatrixShadow;
	ID3DX11EffectShaderResourceVariable* shadowMapShaderResourceView;
	ID3D11ShaderResourceView* shadowMap;
	DirectionalLightClass* directionalLight;
};

#endif 