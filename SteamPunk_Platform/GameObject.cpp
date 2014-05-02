#include "GameObject.h"

GameObject::GameObject(D3DXMATRIX w, ModelClass::BoundingBox b, bool iBack, ObjectType oType)
{
	world = w;
	bBox = b;
	//bufferIndices = i;
	isBackground = iBack;
	type = oType;
}

GameObject::GameObject(const GameObject& other)
{


}

GameObject::~GameObject()
{

}

void GameObject::Initialize()
{

}

void GameObject::Shutdown()
{

}

void GameObject::Update()
{


}