//
//Door.h
//

#ifndef _DOOR_H_
#define _DOOR_H_

#include "GameObject.h"

class Door : public GameObject
{
	
public:
	Door(D3DXMATRIX, ModelClass::BoundingBox, bool, ObjectType);
	~Door();

	void Update();
	void Initialize();
	void Shutdown();
};
#endif