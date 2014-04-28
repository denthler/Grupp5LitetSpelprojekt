////////////////////////////////////////////////////////////////////////////////
// Filename: model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	//m_Texture = 0;
	subsetCount = 0;
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

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* Filename)
{
	bool result;

	std::vector<std::wstring> textureNameArray;

	result = LoadDataStructures(Filename, textureNameArray);
	if(!result)
	{
		return false;
	}

	for(int i = 0; i < subset.size(); i++)
	{
		if(subset[i].vertices.size() > 0)
		{
		result = InitializeBuffers(device, i);
		if(!result)
		{
			return false;
		}
		
	
		if(subset[i].mat.hasTexture)
		{
			const WCHAR* temp = textureNameArray[subset[i].mat.texArrayIndex].c_str();
			result = LoadTexture(device, temp, i);
			if(!result)
			{
				return false;
			}
		}
		}
	}
	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();

	return;
}

void ModelClass::Apply(ID3D11DeviceContext* deviceContext, int i)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &subset[i].vertBuff, &stride, &offset);

	deviceContext->IASetIndexBuffer(subset[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture[0]->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device, int i)
{

	VertexType* verticesTemp;
	unsigned long* indicesTemp;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	

	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * subset[i].vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0; 

	ZeroMemory( &vertexData, sizeof(vertexData) );
	vertexData.pSysMem = &subset[i].vertices[0];
	result = device->CreateBuffer( &vertexBufferDesc, &vertexData, &subset[i].vertBuff);

	if(result)
	{
		return false;
	}
	
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) *  subset[i].indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	ZeroMemory( &indexData, sizeof(indexData) );
	indexData.pSysMem = &subset[i].indices[0];
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &subset[i].indexBuff);

	if(result)
	{
		return false;
	}
	
	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename, int i)
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
}

void ModelClass::ReleaseTexture()
{

	for(int i = 0; i < subset.size(); i++)
	{
		if(subset[i].m_Texture)
		{
			subset[i].m_Texture->Shutdown();
			delete subset[i].m_Texture;
			subset[i].m_Texture = 0;
		}
	}
}

void ModelClass::ShutdownBuffers()
{

	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool ModelClass::Update(std::vector<BoundingBox>& bb)
{
	D3DXVECTOR3 temp;
	bool res = false;
	float time = 1.0f;
	D3DXVec3Cross(&temp, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	velocity += gravity * worldAxis;

	D3DXVECTOR3 moveAmount = velocity; //* gameTime;

	if ((temp.x) > 0.0f || (temp.y) > 0.0f || (temp.z) > 0.0f)
		res = HorizontalCollisionTest(moveAmount, bb, -1.1f);
	else
		res = HorizontalCollisionTest(moveAmount, bb, 1.1f);

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
bool ModelClass::VerticalCollisionTest(D3DXVECTOR3& amount, std::vector<BoundingBox>& bb)
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

bool ModelClass::LoadDataStructures(std::wstring filename, std::vector<std::wstring>& textureNameArray)
{

	std::vector<FaceType> faces;

	std::wifstream fin;
	std::wstring junk;
	int faceIndex, vertexIndex, tIndex, nIndex;
	int triangleCount;
	wchar_t character, input2;
	std::ofstream fout;
	std::vector<D3DXVECTOR3> vertPos;
	std::vector<D3DXVECTOR3> vertNorm;
	std::vector<D3DXVECTOR2> vertTexCoord;
	std::vector<std::wstring> meshMaterials;
	std::vector<int> tempindices;
	

	std::wstring meshMatLib;

	faceIndex = 0;
	vertexIndex = 0;
	triangleCount = 0;
	subsetCount = 0;

	fin.open(filename);
	
	
	if(fin.fail() == true)
	{
		return false;
	}
	

	character = fin.get();
	while(!fin.eof())
	{
		switch(character)
		{
		case'#':
		{
				character = fin.get();
				while(character != L'\n')
					character = fin.get();
				break;
		}
		case 'v':
		{

			character = fin.get();

			if(character == ' ')
			{
				float vx, vy, vz;
				fin >> vx >> vy >> vz;

				vertPos.push_back(D3DXVECTOR3(vx, vy, vz * - 1.0f));				
			}

			if(character == 't')
			{
				float tu, tv;
				fin >> tu >> tv;

				vertTexCoord.push_back(D3DXVECTOR2(tu, 1.0f - tv));
			}

			if(character == 'n')
			{
				float nx, ny, nz;
				fin >> nx >> ny >> nz;

				vertNorm.push_back(D3DXVECTOR3(nx, ny, nz * - 1.0f));
			}
			break;
		}
		case 'g':
		{
			character = fin.get();
			if(character == ' ')
			{
				subsetIndexStart.push_back(vertexIndex);
				subsetCount++;
			}
			SubsetType tempSub;
			subset.push_back(tempSub);
			break;
		}

		case 'f':
		{
			if(subsetCount == 0)
			{
				subsetIndexStart.push_back(vertexIndex);
				subsetCount++;
				SubsetType tempSub;
				subset.push_back(tempSub);
			}

			character = fin.get();
			if(character == ' ')
			{
				float v, t, n;
				
				faces.push_back(FaceType());
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3;
				tempindices.push_back(faces[faceIndex].vIndex3);
				tempindices.push_back(faces[faceIndex].tIndex3);
				tempindices.push_back(faces[faceIndex].nIndex3);
				//indices.push_back((unsigned long)(faces[faceIndex].vIndex3 - 1));
				vertexIndex++;
				fin	>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2;
				tempindices.push_back(faces[faceIndex].vIndex2);
				tempindices.push_back(faces[faceIndex].tIndex2);
				tempindices.push_back(faces[faceIndex].nIndex2);
				vertexIndex++;
				//indices.push_back((unsigned long)(faces[faceIndex].vIndex2 - 1));
				fin	>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				tempindices.push_back(faces[faceIndex].vIndex1);
				tempindices.push_back(faces[faceIndex].tIndex1);
				tempindices.push_back(faces[faceIndex].nIndex1);
				//indices.push_back((unsigned long)(faces[faceIndex].vIndex1 - 1));
				vertexIndex++;
				faceIndex++;
				
			}
			triangleCount++;
			break;
		}

		case 'm':
		{
			fin >> character;
			if(character == 't')
			{
			fin >> character;
			fin >> character;
			fin >> character;
			fin >> character;
			
			character = fin.get();
			if(character == ' ')
			{
				character = fin.get();
				while((character != L' ') && (character != '\n'))
				{
					meshMatLib += character;
					character = fin.get();
				}
			}
			}
			break;
		}

		case 'u':
		{
			fin >> character;
			if(character == 's')
			{
			fin >> character;
			fin >> character;
			fin >> character; 
			fin >> character;
			character = fin.get();					
			if(character == ' ')
			{
				std::wstring meshMaterialsTemp = L"";

				character = fin.get();
				while((character != ' ') && (character != '\n'))
				{
					meshMaterialsTemp += character;
					character = fin.get();
				}

				meshMaterials.push_back(meshMaterialsTemp);
				subset[subsetCount-1].mat.name = meshMaterialsTemp;
			}
			}
			break;
		}

		}
		while(character != '\n')
		{
			character = fin.get();
		}
		

		character = fin.get();
	}

	subsetIndexStart.push_back(vertexIndex);

	if(subsetIndexStart[1] == 0)
	{

		subsetIndexStart.erase(subsetIndexStart.begin() + 1);
		subsetCount--;
	}

	fin.close();
	fin.open(meshMatLib);

	int matCount = material.size();
	m_vertexCount = vertexIndex;

	bool kdset = false;

	if(fin.fail() == true)
	{
		return false;
	}

	while(!fin.eof())
	{
		character = fin.get();
		switch(character)
		{
		case 'K':
		{
			character = fin.get();
			if(character == 'd')
			{
				character = fin.get();
				
				//fin >> material[matCount - 1].difColor.x;
				//fin >> material[matCount - 1].difColor.y;
				//fin >> material[matCount - 1].difColor.z;
				fin >> subset[matCount - 1].mat.difColor.x;
				fin >> subset[matCount - 1].mat.difColor.y;
				fin >> subset[matCount - 1].mat.difColor.z;
			}
			if(character == 'a')
			{
				character = fin.get();
				
				//fin >> material[matCount - 1].ambColor.x;
				//fin >> material[matCount - 1].ambColor.y;
				//fin >> material[matCount - 1].ambColor.z;
				fin >> subset[matCount - 1].mat.ambColor.x;
				fin >> subset[matCount - 1].mat.ambColor.y;
				fin >> subset[matCount - 1].mat.ambColor.z;
			}
			break;
		}
		case 'T':
		{
			character = fin.get();
			if(character == 'r')
			{
				character = fin.get();
				float Transparency;
				fin >> Transparency;

				material[matCount - 1].difColor.w = Transparency;
				subset[matCount - 1].mat.difColor.w = Transparency;
			}
			break;
		}
		case 'd':
		{
			character = fin.get();
			if(character == ' ')
			{
				float Transparency;
				fin >> Transparency;

				Transparency = 1.0f - Transparency;

				material[matCount - 1].difColor.w = Transparency;
				subset[matCount - 1].mat.difColor.w = Transparency;
			}
			break;
		}

		case 'm':
		{
			fin >> character;
			if(character == 'a')
			{
			fin >> character;
			fin >> character; 
			fin >> character;
			fin >> character;
			//input = fin.get();					
			if(character == 'd')
			{
				std::wstring fileNamePath = L"";


				character = fin.get();
				bool done = false;
				while(!done)
				{
					if((character != ' ') && (character !='\n'))
						fileNamePath += character;

					character = fin.get();

					if(character == '.')
					{
						fileNamePath += character;
						for(int i = 0; i < 3; ++i)
						{
							character = fin.get();
							fileNamePath += character;
						}
										done = true;
					}

				}
					subset[matCount - 1].mat.hasTexture = true;
					material[matCount - 1].texArrayIndex = textureNameArray.size();
					subset[matCount - 1].mat.texArrayIndex = textureNameArray.size();
					textureNameArray.push_back(fileNamePath.c_str());
					material[matCount - 1].hasTexture = true;
				
			}
			}
			break;
		}
		case 'n':
		{
			fin >> character;
			fin >> character;
			fin >> character; 
			fin >> character;
			fin >> character;
			character = fin.get();

			if(character == ' ')
			{
				Material temp;
				material.push_back(temp);
				std::wstring meshMaterialsTemp = L"";

				character = fin.get();
				while((character != ' ') && (character != '\n'))
				{
					meshMaterialsTemp += character;
					character = fin.get();
				}

				material[matCount].name = meshMaterialsTemp;
				material[matCount].hasTexture = false;
				material[matCount].texArrayIndex = 0;
				subset[matCount].mat.hasTexture = false;
				subset[matCount].mat.name = meshMaterialsTemp;
				matCount++;
			}
			break;
		}
	}
	}
	for(int i = 0; i < subsetCount; ++i)
	{
		bool hasMat = false;
		for(int j = 0; j < (int)material.size(); ++j)
		{
			if(meshMaterials[i].c_str() == material[j].name.c_str())
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if(!hasMat)
		{
			subsetMaterialArray.push_back(0);
		}
	}

	VertexType tempVert;
	int k = 0;
	/*
	for(int i = 0; i < (subsetIndexStart.size() - 1); i++)
	{
		int m = 0;
		//SubsetType tempSub;
		for(int j = subsetIndexStart[i]; j < (subsetIndexStart[i+1]); j++)
		{
			VertexType v;
			v.position = D3DXVECTOR3(vertPos[tempindices[k] - 1].x, vertPos[tempindices[k] - 1].y, vertPos[tempindices[k] - 1].z);
			v.normal = D3DXVECTOR3(vertNorm[tempindices[k + 1] - 1].x, vertNorm[tempindices[k + 1] - 1].y, vertNorm[tempindices[k + 1] - 1].z);
			v.texture = D3DXVECTOR2(vertTexCoord[tempindices[k + 2] - 1].x, vertTexCoord[tempindices[k + 2] - 1].y);
			subset[i].vertices.push_back(v);
			subset[i].indices.push_back((unsigned long)m);
			m++;
			k += 2;
		}
		//subset.push_back(tempSub);
	}
	*/
	
	for(int j = 0; j < (subsetIndexStart.size() - 1); j++)
	{
		k = 0;
		int numFaces = ((subsetIndexStart[j+1] - subsetIndexStart[j]) / 3);
		int prevNumFaces = (subsetIndexStart[j] / 3);
	for(int i = prevNumFaces; i < prevNumFaces + numFaces; i++)
	{
		
		vertexIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		

		tempVert.position = D3DXVECTOR3(vertPos[vertexIndex].x, vertPos[vertexIndex].y, vertPos[vertexIndex].z);
		tempVert.normal = D3DXVECTOR3(vertNorm[nIndex].x, vertNorm[nIndex].y, vertNorm[nIndex].z);
		tempVert.texture = D3DXVECTOR2(vertTexCoord[tIndex].x, vertTexCoord[tIndex].y);

		vertices.push_back(tempVert);
		indices.push_back((unsigned long)k);
		subset[j].vertices.push_back(tempVert);
		subset[j].indices.push_back((unsigned long)k);
		k++;

		vertexIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		
		tempVert.position = D3DXVECTOR3(vertPos[vertexIndex].x, vertPos[vertexIndex].y, vertPos[vertexIndex].z);
		tempVert.normal = D3DXVECTOR3(vertNorm[nIndex].x, vertNorm[nIndex].y, vertNorm[nIndex].z);
		tempVert.texture = D3DXVECTOR2(vertTexCoord[tIndex].x, vertTexCoord[tIndex].y);
		
		vertices.push_back(tempVert);
		indices.push_back((unsigned long)k);
		subset[j].vertices.push_back(tempVert);
		subset[j].indices.push_back((unsigned long)k);
		k++;
		
		vertexIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		
		
		tempVert.position = D3DXVECTOR3(vertPos[vertexIndex].x, vertPos[vertexIndex].y, vertPos[vertexIndex].z);
		tempVert.normal = D3DXVECTOR3(vertNorm[nIndex].x, vertNorm[nIndex].y, vertNorm[nIndex].z);
		tempVert.texture = D3DXVECTOR2(vertTexCoord[tIndex].x, vertTexCoord[tIndex].y);
		
		vertices.push_back(tempVert);
		indices.push_back((unsigned long)k);
		subset[j].vertices.push_back(tempVert);
		subset[j].indices.push_back((unsigned long)k);
		k++;
	}
	}
	
	m_vertexCount = vertices.size();
	m_indexCount = triangleCount * 3;

	/*
	float minXLength = 9000.0f;
	float maxXLength = 0.0f;
	float minYLength = 9000.0f;
	float maxYLength = 0.0f;
	float minZLength = 9000.0f;
	float maxZLength = 0.0f;
	*/
	bBox.max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bBox.min = D3DXVECTOR3(9000.0f, 9000.0f, 9000.0f);
	for(int j = 0; j < subset.size(); j++)
	{
		for(int k = 0; k < subset[j].vertices.size(); k++)
		{
			//tempLength = D3DXVec3Length(&subset[j].vertices[k].position);
			/*
			if(tempLength < minLength)
			{
				bBox.min = subset[j].vertices[k].position;
			}
			else if(tempLength > maxLength)
			{
				bBox.max = subset[j].vertices[k].position;
			}
			*/
			bBox.max.x = max(bBox.max.x, subset[j].vertices[k].position.x);
			bBox.min.x = min(bBox.min.x, subset[j].vertices[k].position.x);

			bBox.max.y = max(bBox.max.y, subset[j].vertices[k].position.y);
			bBox.min.y = min(bBox.min.y, subset[j].vertices[k].position.y);

			bBox.max.z = max(bBox.max.z, subset[j].vertices[k].position.z);
			bBox.min.z = min(bBox.min.z, subset[j].vertices[k].position.z);
		}
	}

	return true;
}