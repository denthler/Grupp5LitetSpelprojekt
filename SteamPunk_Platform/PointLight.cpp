////////////////////////////////////////////////////////////////////////////////
// Filename: pointlight.cpp
////////////////////////////////////////////////////////////////////////////////
#include "pointlight.h"


PointLightClass::PointLightClass()
{
}


PointLightClass::PointLightClass(const PointLightClass& other)
{
}


PointLightClass::~PointLightClass()
{
}


void PointLightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void PointLightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
	return;
}

void PointLightClass::SetRange(float r)
{
	m_range = r;
}

void PointLightClass::SetAttenuation(float x, float y, float z)
{
	m_att = D3DXVECTOR3(x, y, z);
	return;
}

void PointLightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

D3DXVECTOR4 PointLightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 PointLightClass::GetAttenuation()
{
	return m_att;
}

float PointLightClass::GetRange()
{
	return m_range;
}

D3DXVECTOR4 PointLightClass::GetAmbientColor()
{
	return m_ambientColor;
}


D3DXVECTOR4 PointLightClass::GetPosition()
{
	return m_position;
}