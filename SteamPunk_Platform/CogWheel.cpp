//
//CogWheel.cpp
//

#include "CogWheel.h"

Gear::Gear(D3DXMATRIX w, ModelClass::BoundingBox b, bool iBack, ObjectType t) : GameObject(w, b, iBack, t)
{
	rotation = 0.0f;
}

Gear::~Gear()
{

}

void Gear::Initialize()
{

}

void Gear::Shutdown()
{

}

void Gear::Update()
{
	rotation += 0.01f;
	if (rotation > 360.0f)
		rotation -= 360.0f;
	
	D3DXMATRIX rot;
	D3DXMatrixRotationY(&rot, rotation);

	rot._41 = world._41;
	rot._42 = world._42;
	rot._43 = world._43;

	world = rot;
}

