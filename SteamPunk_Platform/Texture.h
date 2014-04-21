#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <D3DX11tex.h>


class TextureClass
{

public: 
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif
