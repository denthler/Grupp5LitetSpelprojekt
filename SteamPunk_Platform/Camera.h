//
//Camera.h
//
#ifndef CAMERA_H_
#define CAMERA_H_

#include <d3d11.h>
#include <D3DX10math.h>

class Camera
{

public:
	Camera();
	~Camera();

	bool Initialize(float x, float y, float z, D3DXVECTOR3);
	void Flip();
	void FlipS();
	D3DXMATRIX GetView()
	{
		return viewMatrix;
	}
	D3DXMATRIX Getworld()
	{
		return worldMatrix;
	}
private:
	D3DXMATRIX viewMatrix;
	D3DXMATRIX worldMatrix;
};
#endif
