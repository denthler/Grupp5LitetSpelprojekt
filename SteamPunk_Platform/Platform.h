//
//Platform.h
//
#ifndef _Platform_H_
#define _Platform_H_

#include "GameObject.h"

class Platform : public GameObject
{
public:
	Platform(D3DXMATRIX, ModelClass::BoundingBox, bool);
	Platform(const Platform&);
	~Platform();

	void Update();
	void Initialize();
	void Shutdown();

};
#endif