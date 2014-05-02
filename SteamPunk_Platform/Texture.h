#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <D3DX11tex.h>
#include <vector>

class TextureClass
{

public: 
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, std::string);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif
