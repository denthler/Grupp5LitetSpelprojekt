////////////////////////////////////////////////////////////////////////////////
// Filename: DirectionalLight.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

class DirectionalLightClass
{
public:
	DirectionalLightClass();
	~DirectionalLightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetAttenuation(float, float, float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR3 GetAttenuation();
private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR3 m_att;
};
#endif