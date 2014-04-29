//
//GameObject.h
//
#ifndef _GameObject_H_
#define _GameObject_H_

#include "model.h"

class GameObject
{
public:
	GameObject(D3DXMATRIX, ModelClass::BoundingBox, bool);
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

private:
	D3DXMATRIX world;
	ModelClass::BoundingBox bBox;
	bool isBackground;
	//std::vector<int> bufferIndices;
	//int vCount;
};
#endif
