//
//CogWheel.ch
//

#ifndef _COG_H_
#define _COG_H_

#include "GameObject.h"

class Gear : public GameObject
{
	
public:
	Gear(D3DXMATRIX, ModelClass::BoundingBox, bool, ObjectType);
	~Gear();

	void Update();
	void Initialize();
	void Shutdown();

private:
	float rotation;

};
#endif