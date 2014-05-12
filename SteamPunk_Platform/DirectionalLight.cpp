////////////////////////////////////////////////////////////////////////////////
// Filename: pointlight.cpp
////////////////////////////////////////////////////////////////////////////////
#include "DirectionalLight.h"


DirectionalLightClass::DirectionalLightClass()
{
}

DirectionalLightClass::~DirectionalLightClass()
{
}


void DirectionalLightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void DirectionalLightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void DirectionalLightClass::SetAttenuation(float x, float y, float z)
{
	m_att = D3DXVECTOR3(x, y, z);
	return;
}

void DirectionalLightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

D3DXVECTOR4 DirectionalLightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 DirectionalLightClass::GetAttenuation()
{
	return m_att;
}

D3DXVECTOR4 DirectionalLightClass::GetAmbientColor()
{
	return m_ambientColor;
}


D3DXVECTOR3 DirectionalLightClass::GetDirection()
{
	return m_direction;
}