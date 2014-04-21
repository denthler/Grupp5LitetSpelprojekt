//
//Camera.cpp
//

#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}
void Camera::Flip()
{
	Initialize(0.0f, 10.0f, -50.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
}
void Camera::FlipS()
{
	Initialize(0.0f, 10.0f, -50.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}
bool Camera::Initialize(float x, float y, float z, D3DXVECTOR3 up)
{
	D3DXVECTOR3 vec, dir;
	vec = D3DXVECTOR3(x, y, z);
	dir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrix, x, y, z);
	//worldMatrix = DirectX::XMMatrixTranslation(x, y, z);

	D3DXMatrixLookAtLH(&viewMatrix, &vec, &dir, &up);
	//viewMatrix = DirectX::XMMatrixLookToLH(vec, dir, up);
	//viewMatrix = DirectX::XMMatrixMultiply(viewMatrix, worldMatrix);

	//iewMatrix = viewMatrix * worldMatrix;
	return true;
}