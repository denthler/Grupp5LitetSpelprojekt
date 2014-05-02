
#include "texture.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, std::string filename)
{

	HRESULT result;

	std::wstring tempS = std::wstring(filename.begin(), filename.end());
	LPCWSTR sw = tempS.c_str();
	result = D3DX11CreateShaderResourceViewFromFile(device, sw, NULL, NULL, &m_texture, NULL); 
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{

	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}


