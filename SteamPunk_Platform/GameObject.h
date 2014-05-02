//
//GameObject.h
//
#ifndef _GameObject_H_
#define _GameObject_H_

#include "model.h"

class GameObject
{
public:
	enum ObjectType
	{
		Background = 1,
		Platform = 2,
		Gear = 4
	};

	GameObject(D3DXMATRIX, ModelClass::BoundingBox, bool, ObjectType);
	GameObject(const GameObject&);
	~GameObject();

	virtual void Update();
	virtual void Initialize();
	virtual void Shutdown();

	D3DXMATRIX getWorld()
	{
		return world;
	}
	ModelClass::BoundingBox getBoundingBox()
	{
		return bBox;
	}
	bool IsBackground()
	{
		return isBackground;
	}
	ObjectType GetType()
	{
		return type;
	}
	D3DXVECTOR3 GetPosition()
	{
		D3DXVECTOR3 pos;
		pos = D3DXVECTOR3(world._41, world._42, world._43);
		return pos;
	}
protected:
	D3DXMATRIX world;
	ObjectType type;
private:
	ModelClass::BoundingBox bBox;
	bool isBackground;
	//std::vector<int> bufferIndices;
	//int vCount;
};
#endif
