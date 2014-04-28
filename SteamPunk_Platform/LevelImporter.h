#ifndef _LEVELIMPORTER_H_
#define _LEVELIMPORTER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <D3DX10math.h>

struct Type
{
	std::string type;
	std::vector<D3DXMATRIX> transforms;
};

class LevelImporter
{
public:
	LevelImporter();
	~LevelImporter();

	bool					LoadLevel(std::string level);
	std::vector<Type*>		types;
private:
	std::string				file_Path;
	Type*					currentType;
};

#endif