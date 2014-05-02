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

	bool Initialize(ID3D11Device*, D3DXVECTOR3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, std::vector<AnimationStack>, ID3D11Buffer*, int);
	bool Update(float, std::vector<BoundingBox>&);
	//void Kill();
	//void Revive();
	void Jump();
	void Shutdown();
	bool IsDead()
	{
		return dead;
	}
	bool IsOnGround()
	{
		return OnGround;
	}
	D3DXMATRIX GetWorldMatrix();

	void SetLeft(){left = true;}
	void SetRight(){right = true;}
	void SetDown(){left = true;}
	void SetUp(){left = true;}
	void SetJump(){jump = true;}
	void Kill();
	D3DXVECTOR3 GetPosition(){return position;}
	//void FlipGravity(){D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);}
		//worldAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); }
	//void FlipGravityS(){D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));}
		//worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f); }
private:

	bool dead;
	D3DXVECTOR3 StartPos;
	bool left, right, up, down, jump;

};

#endif