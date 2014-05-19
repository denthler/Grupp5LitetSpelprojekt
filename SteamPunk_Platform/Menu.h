#ifndef _MENU_H_
#define _MENU_H_

#include <vector>
#include <d3d11.h>
#include <D3DX10math.h>

#include "AssetImporter.h"
#include "Texture.h"
#include "model.h"
#include "Render.h"

struct MenuMesh
{
	ID3D11Buffer* m_vertexBuffer;
	D3DXMATRIX world_Matrix;

	ID3D11ShaderResourceView* textureMap;
	ID3D11ShaderResourceView* normalMap;
	int vCount;
};


class Menu
{
public:
	Menu(ID3D11Device*);
	~Menu();

	void						Update(D3DXMATRIX playerPos, bool Rotated, D3DXVECTOR3 worldAxis);
	void						UpdatePosition(D3DXMATRIX playerPos, bool Rotated, D3DXVECTOR3 worldAxis);

	void						Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ModelClass::Material);
	void						Pause();

private:
	void						CreateVertexBuffer(ID3D11Device* device);
public:
	bool						pause;	
private:
	AssetImporter				aImporter;
	TextureClass				texture;
	MenuMesh					mesh;
	bool						notMoving;
	float						offset;
};

#endif