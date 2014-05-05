//
//Camera.cpp
//

#include "Camera.h"

Camera::Camera()
{
	currentUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	targetUp = currentUp;
	s = 1.0f;
}

Camera::~Camera()
{

}

void Camera::Update(D3DXVECTOR3 playerPosition)
{
	// Rotate camera
	if(targetUp != currentUp && s < 1.0f)
	{
		s += 0.01f;
		D3DXVECTOR3 temp;	
		D3DXVec3Lerp(&temp, &currentUp, &targetUp, s);
		Initialize(viewMatrix._41, viewMatrix._42, -50.0f, temp);
		currentUp = temp;
	}

	// Camera movement. It follows the player's movement.
	Initialize(playerPosition.x, playerPosition.y, playerPosition.z, currentUp);
}
void Camera::Flip()
{
	D3DXVec3Cross(&targetUp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &currentUp);
	s = 0.0f;
}

void Camera::FlipS()
{
	D3DXVec3Cross(&targetUp, &currentUp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	s = 0.0f;
}

bool Camera::Initialize(float x, float y, float z, D3DXVECTOR3 up)
{
	D3DXVECTOR3 target(x, y, z);
	D3DXVECTOR3 cameraPos(x, y, -50.0f);
	D3DXMatrixTranslation(&worldMatrix, x, y, z);
	D3DXMatrixLookAtLH(&viewMatrix, &cameraPos, &target, &up);
	return true;
}