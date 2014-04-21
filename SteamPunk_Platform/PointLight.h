////////////////////////////////////////////////////////////////////////////////
// Filename: pointlight.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

class PointLightClass
{
public:
	PointLightClass();
	PointLightClass(const PointLightClass&);
	~PointLightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetAttenuation(float, float, float);
	void SetRange(float);
	

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR3 GetAttenuation();
	float GetRange();
private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_position;
	float m_range;
	D3DXVECTOR3 m_att;
};
#endif