#pragma once
#include "Section.h"
#include "Object.h"

class Field
{
public:
	Field();
	Field(std::string &fileName, std::vector<Object*>* _OBJ, unsigned int _WIDTH, unsigned int _HEIGHT, float SECT_HEIGHT, float SECT_WIDTH, std::vector<Object*>* plateObj, std::vector<Object*>* players);
	Section* getSection(int _X, int _Y);
	~Field();
private:
	std::vector<std::vector<Section*>> grid;
	unsigned int height, width;
	float sectHeight, sectWidth;
	void loadFromFile(std::string &fileName, std::string & dir, std::vector<Object*>* _OBJ, std::vector<Object*>* bases, std::vector<Object*>* players);
};