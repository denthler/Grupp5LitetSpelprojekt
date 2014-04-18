#include "OBJ.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace DirectX;

void Object::LoadFromObjFile(string dir, string filename)
{
	vector<XMFLOAT3> tempPoints;
	vector<XMFLOAT2> tempTexCoords;
	vector<XMFLOAT3> tempNormals;
	vector<ObjectMaterialData> tempMaterialData;

	int lastGroupIteration = 0;
	int iterationCount = 0;
	bool ignorePreviousGroup  = false;

	ifstream f(dir + filename);
	if(!f)
	{
		cout << "Can't open file: " << dir + filename << endl;
		return;
	}
	
	string str;
	while(!f.eof())
	{
		f >> str;
		if(str == "#")
		{
			f.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if(str == "v") 
		{	
			XMFLOAT3 u;			
			f >> u.x;
			f >> u.y;
			f >> u.z;
			tempPoints.push_back(u);
		}
		else if(str == "vt") 
		{	
			XMFLOAT2 u;
			f >> u.x;
			f >> u.y;
			tempTexCoords.push_back(u);
		}
		else if(str == "vn")
		{
			XMFLOAT3 n;
			f >> n.x;
			f >> n.y;
			f >> n.z;
			tempNormals.push_back(n);
		}
		else if(str == "f")
		{
			if(lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}

			unsigned int indices[9];
			for(int i = 0; i < 9; i++)
			{
				f >> indices[i];
				indices[i] -= 1;
				f.ignore();
			}

			for(int i = 0; i < 3; i++)
			{				
				int index = i * 3;
				unsigned int pointIndex = indices[index];
				unsigned int texCoordIndex = indices[index + 1];
				unsigned int normalIndex = indices[index + 2];
				VertexData vert;				
				vert.point = tempPoints[pointIndex];
				vert.texCoord = tempTexCoords[texCoordIndex];
				vert.normal = tempNormals[normalIndex];		
				groups.back().vertices.push_back(vert);				
			}
		}
		else if(str == "s")
		{
			f.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if(str == "mtllib")
		{
			if(lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}
			string s;
			f >> s;
			//LoadMaterialData("../../bth_logo_obj_tga/" + s, tempMaterialData);
			LoadMaterialData(dir + s, tempMaterialData);
		}
		else if(str == "usemtl")
		{
			if(lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}
			string mtlname;
			f >> mtlname;
			for(auto it = tempMaterialData.begin(); it != tempMaterialData.end(); it++)
			{
				if(it->Name == mtlname)
				{
					groups.back().materialData = *it;
					break;
				}
			}					
		}
		else if(str == "g")
		{
			lastGroupIteration = iterationCount;
			if(ignorePreviousGroup)
			{
				f >> groups.back().name;
			}
			else
			{
				Group g;
				f >> g.name;			
				groups.push_back(g);
				ignorePreviousGroup = true;
			}
		}
		else
		{
			string s;
			std::getline(f, s);
			cout << str << s << endl;			
		}
		iterationCount++;
	}	
}

void Object::LoadMaterialData(std::string filename, vector<ObjectMaterialData> & data)
{
	ifstream f(filename);
	if(!f)
	{
		cout << "Can't open file: " << filename << endl;
		return;
	}
	
	string str;
	while(!f.eof())
	{
		f >> str;
		if(str == "newmtl")
		{
			ObjectMaterialData mtl;
			f >> mtl.Name;
			data.push_back(mtl);
		}
		else if(str == "illum")
		{
			f >> data.back().Illum;
		}
		else if(str == "Kd")
		{		
			f >> data.back().Kd[0];	
			f >> data.back().Kd[1];
			f >> data.back().Kd[2];
		}
		else if(str == "Ka")
		{
			f >> data.back().Ka[0];
			f >> data.back().Ka[1];
			f >> data.back().Ka[2];
		}
		else if(str == "Tf")
		{
			f >> data.back().Tf[0];
			f >> data.back().Tf[1];
			f >> data.back().Tf[2];
		}
		else if(str == "map_Kd")
		{
			f >> data.back().map_Kd;
		}
		else if(str == "Ni")
		{
			f >> data.back().Ni;
		}
		else
		{
			string s;
			std::getline(f, s);
			cout << str << s << endl;			
		}
	}
}

/* 
void Object::Init()
{
	// Maybe fix so multiple groups can be handled?
	glGenBuffers(1, bufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, GetVertexDataSize(0), GetVertexData(0), GL_STATIC_DRAW);

	glGenVertexArrays(1, VAOHandle);
	glBindVertexArray(VAOHandle[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(5 * sizeof(float)));
}

void Object::Draw()
{
	glBindVertexArray(VAOHandle[0]);
	glDrawArrays(GL_TRIANGLES, 0, GetVertexCount(0));
}

Object::~Object()
{
	glDeleteBuffers(1, bufferHandle);
	glDeleteVertexArrays(1, VAOHandle);
}
*/