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

	void Update(D3DXVECTOR3 playerPosition);
	bool Initialize(float x, float y, float z, D3DXVECTOR3);
	void Flip();
	void FlipS();
	void Reset()
	{
		targetUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		currentUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	D3DXMATRIX GetView()
	{
		return viewMatrix;
	}
	D3DXMATRIX Getworld()
	{
		return worldMatrix;
	}
	D3DXVECTOR3 GetPosition() { return D3DXVECTOR3(viewMatrix._41, viewMatrix._42, viewMatrix._43); }

	bool IsNotFlipping() { return s == 1.0f; }
private:
	D3DXMATRIX viewMatrix;
	D3DXMATRIX worldMatrix;
	float s;
	D3DXVECTOR3 currentUp;
	D3DXVECTOR3 targetUp;
};
#endif
