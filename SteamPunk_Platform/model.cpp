////////////////////////////////////////////////////////////////////////////////
// Filename: model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"

ModelClass::ModelClass()
{
	//m_Texture = 0;
	D3DXMatrixIdentity(&m_worldMatrix);

	moveScale = -0.1f;
	gravity = -0.001f;
	worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	OnGround = false;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}
void ModelClass::GetWorldMatrix(D3DXMATRIX& world)
{
	world = m_worldMatrix;
}

bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	mat.ambColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	mat.difColor = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	mat.hasTexture = false;
	mat.hasNormal = false;

	return true;
}

void ModelClass::Shutdown()
{
	return;
}

void ModelClass::Apply(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexTypeAni);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename, int i)
{

	bool result;

	TextureClass* TempTexture = new TextureClass();
	if(!TempTexture)
	{
		return false;
	}

	result = TempTexture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}
	m_Texture.push_back(TempTexture);
	subset[i].m_Texture = TempTexture;
	return true;
}*/

bool ModelClass::Update(float time, std::vector<BoundingBox>& bb)
{
	D3DXVECTOR3 temp;
	bool res = false;
	//float time = 1.0f;
	D3DXVec3Cross(&temp, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	velocity += gravity * worldAxis;

	D3DXVECTOR3 moveAmount = velocity; //* gameTime;

	if ((temp.x) > 0.0f || (temp.y) > 0.0f || (temp.z) > 0.0f)
		res = HorizontalCollisionTest(moveAmount, bb, -1.1f);//-1.1f
	else
		res = HorizontalCollisionTest(moveAmount, bb, 1.1f);//1.1f

	if (!(VerticalCollisionTest(moveAmount, bb)) && !(res) && !(OnGround))
	{
		res = true;
	}

	if (((abs(worldAxis.x) * moveAmount.x) != 0.0f) ||
		((abs(worldAxis.y) * moveAmount.y) != 0.0f) ||
		((abs(worldAxis.z) * moveAmount.z) != 0.0f))
	{
		OnGround = false;
	}
	else
	{
		OnGround = true;
	}
	D3DXVECTOR3 newPosition = position + (moveAmount * (time));
	position = newPosition;
	return res;
}

bool ModelClass::HorizontalCollisionTest(D3DXVECTOR3& amount, std::vector<BoundingBox>& bb, float value)
{
	bool result = false;
	D3DXVECTOR3 temp;
	D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	D3DXVECTOR3 offset;

	offset.x = position.x + ((amount.x * temp.x) * value);
	offset.y = position.y + ((amount.y * temp.y)* value);
	offset.z = position.z + ((amount.z * temp.z)* value);

	D3DXVECTOR3 tempMax = bBox.max;
	D3DXVECTOR3 tempMin = bBox.min;

	tempMax = offset + tempMax;
	tempMin = offset + tempMin;

	for (int j = 0; j < bb.size(); j++)
	{
		if (tempMax.x > bb[j].min.x &&
			tempMin.x < bb[j].max.x &&
			tempMax.y > bb[j].min.y &&
			tempMin.y < bb[j].max.y &&
			tempMax.z > bb[j].min.z &&
			tempMin.z < bb[j].max.z)
		{
			if ((abs(temp.x) * amount.x) != 0.0f)
			{
				amount.x = 0.0f;
				result = true;
			}
			if ((abs(temp.y) * amount.y) != 0.0f)
			{
				amount.y = 0.0f;
				result = true;
			}
			if ((abs(temp.z) * amount.z) != 0.0f)
			{
				amount.z = 0.0f;
				result = true;
			}
		}
	}

	return result;
}
bool ModelClass::VerticalCollisionTest(D3DXVECTOR3& amount, std::vector<ModelClass::BoundingBox>& bb)
{
	bool result = false;
	D3DXVECTOR3 temp;
	D3DXVECTOR3 offset = position + amount;
	D3DXVECTOR3 tempMax = bBox.max;
	D3DXVECTOR3 tempMin = bBox.min;
	tempMax = offset + tempMax;
	tempMin = offset + tempMin;
	for (int j = 0; j < bb.size(); j++)
	{
		if (tempMax.x > bb[j].min.x &&
			tempMin.x < bb[j].max.x &&
			tempMax.y > bb[j].min.y &&
			tempMin.y < bb[j].max.y &&
			tempMax.z > bb[j].min.z &&
			tempMin.z < bb[j].max.z)
		{
			if ((abs(worldAxis.x) * amount.x) != 0.0f)
			{
				result = true;
				amount.x = 0.0f;
			}
			if ((abs(worldAxis.y) * amount.y) != 0.0f)
			{
				result = true;
				amount.y = 0.0f;
			}
			if ((abs(worldAxis.z) * amount.z) != 0.0f)
			{
				result = true;
				amount.z = 0.0f;
			}
		}
	}
	return false;
}