#include "LevelImporter.h"

LevelImporter::LevelImporter()
{
	file_Path = "Resources/Levels/";
}

LevelImporter::~LevelImporter()
{

}

bool LevelImporter::LoadLevel(std::string level)
{
	std::ifstream inFile;
	file_Path += level;
	
	inFile.open(file_Path);
	if (!inFile.is_open())
	{
		std::wstring debug = L"Could not open: " + std::wstring(level.begin(), level.end());
		OutputDebugString(debug.c_str());
		return false;
	}
		
	std::string str;
	while (!inFile.eof())
	{
		inFile >> str;

		if (str == "type")
		{
			currentType = new Type;
			inFile >> currentType->type;
			types.push_back(currentType);
		}
		else if (str == "mtransform")
		{
			D3DMATRIX newTrans;

			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					inFile >> newTrans.m[y][x];

			currentType->transforms.push_back(newTrans);
		}
	}
	
	inFile.close();
	return true;
}