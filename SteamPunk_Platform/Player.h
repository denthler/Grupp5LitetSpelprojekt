//
//Player.h
//
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "model.h"

class Player : public ModelClass
{
public:
	Player();
	~Player();

	bool Initialize(ID3D11Device*, WCHAR*, D3DXVECTOR3);
	void Update(float, std::vector<BoundingBox>&);
	//void Kill();
	//void Revive();
	void Jump();
	void Shutdown();
	void Apply(ID3D11DeviceContext*, int);
	bool IsDead()
	{
		return dead;
	}
	D3DXMATRIX GetWorldMatrix()
	{
		D3DXMATRIX posMatrix;
		D3DXMatrixTranslation(&posMatrix, position.x, position.y, position.z);
		return posMatrix;
	}
	void SetLeft(){left = true;}
	void SetRight(){right = true;}
	void SetDown(){left = true;}
	void SetUp(){left = true;}
	void SetJump(){jump = true;}
	void FlipGravity(){worldAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);}
	void FlipGravityS(){worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);}
private:
	void VerticalCollisionTest(D3DXVECTOR3&, std::vector<BoundingBox>&);
	
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	bool dead;
	float moveScale;
	float gravity; 
	D3DXVECTOR3 worldAxis; 
	bool left, right, up, down, jump;
	bool OnGround;
};

#endif