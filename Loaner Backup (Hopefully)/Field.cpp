#include "Field.h"
#include "Destructable.h"
#include <fstream>
#include <iostream>

Field::Field()
{
}

Field::Field(std::string &fileName, std::vector<Object*>* _OBJ, unsigned int _WIDTH, unsigned int _HEIGHT, float SECT_HEIGHT, float SECT_WIDTH, std::vector<Object*>* plateObj, std::vector<Object*>* players)
{
	for (int i = 0; i < _WIDTH; i++)
	{
		std::vector<Section*> sub;
		for (int j = 0; j < _HEIGHT; j++)
		{
			Section* sec = new Section();
			sec->setPlane((*plateObj)[0]);
			sub.push_back(sec);
		}
		grid.push_back(sub);
	}
	sectWidth = SECT_WIDTH;
	sectHeight = SECT_HEIGHT;
	width = _WIDTH;
	height = _HEIGHT;
	std::string temp = "Assets/Map/";
	loadFromFile(fileName, temp, _OBJ, plateObj, players);
	for (int i = 0; i < _HEIGHT; i++)
	{
		for (int j = 0; j < _WIDTH; j++)
		{
			std::vector<Section*> nearPointers;
			for (int k = 0; k < 8; k++)
				nearPointers.push_back(nullptr);
			if (i > 0 && j > 0)
			{
				nearPointers[0] = grid[j - 1][i - 1];
			}
			if (i > 0)
			{
				nearPointers[1] = grid[j][i - 1];
			}
			if (i > 0 && j < _WIDTH - 1)
			{
				nearPointers[2] = grid[j + 1][i - 1];
			}
			if (j < _WIDTH - 1)
			{
				nearPointers[3] = grid[j + 1][i];
			}
			if (i < _HEIGHT - 1 && j < _WIDTH - 1)
			{
				nearPointers[4] = grid[j + 1][i + 1];
			}
			if (i < _HEIGHT - 1)
			{
				nearPointers[5] = grid[j][i + 1];
			}
			if (i < _HEIGHT - 1 && j > 0)
			{
				nearPointers[6] = grid[j - 1][i + 1];
			}
			if (j > 0)
			{
				nearPointers[7] = grid[j - 1][i];
			}
			grid[j][i]->setNear(nearPointers);
		}
	}
}

Section * Field::getSection(int _X, int _Y)
{
	return grid[_X][_Y];
}

Field::~Field()
{
}

void Field::loadFromFile(std::string & fileName, std::string & dir, std::vector<Object*>* _OBJ, std::vector<Object*>* bases, std::vector<Object*>* players)
{
	std::string temp;
	std::ifstream theFile((dir + fileName).c_str());
	std::getline(theFile, temp);

	if (temp == "SUBSECTIONS")
	{
		std::getline(theFile, temp);
		std::string blop = dir + temp + "/";
		while (std::getline(theFile, temp))
		{
			loadFromFile(temp, blop, _OBJ, bases, players);
		}
	}
	else if (temp == "TILE")
	{
		std::getline(theFile, temp);
		unsigned int gridSpotX = std::stoi(temp);
		std::getline(theFile, temp);
		unsigned int gridSpotY = std::stoi(temp);
		std::getline(theFile, temp);
		std::string PlateName = temp;
		std::getline(theFile, temp);
		unsigned int tileRot = std::stoi(temp);


		//with these vars, you know the location of the grid space to add objects to
		
		Object* container = new Object();
		Section* tempSec = grid[gridSpotX][gridSpotY];
		container->setPosition(vec3(sectWidth * gridSpotX, 0.f, sectHeight * gridSpotY));
		container->setRotation(vec3(0, tileRot * 90, 0));

		for (int i = 0; i < bases->size(); i++)
		{
			if ((*bases)[i]->getIdentity() == PlateName)
			{
				tempSec->setPlane((*bases)[i]);
			}
		}

		container->AttachObject(tempSec->getPlane());
		tempSec->setPlane(container);
		tempSec->setFilePath(dir + fileName);

		while (std::getline(theFile, temp))
		{
			bool FOUND = false;
			if (temp == "PLACEHOLDER_PLAYER")
			{
				temp = "PLAYER_TRUCK";
				for (int i = 0; i < _OBJ->size() && !FOUND; i++)
				{
					if ((*_OBJ)[i]->getIdentity() == temp)
					{
						FOUND = true;
						//Object* objCont = new Object;
						//objCont->AttachObject((*_OBJ)[i]);
						(*players)[0]->AttachObject((*_OBJ)[i]);

						std::getline(theFile, temp);
						float xLoc = stof(temp);
						std::getline(theFile, temp);
						float yLoc = stof(temp);
						std::getline(theFile, temp);
						int rot = stoi(temp);
						std::getline(theFile, temp);
						float scale = stof(temp);

						(*players)[0]->setPosition(vec3(xLoc, 0.f, yLoc));
						(*players)[0]->setRotation(vec3(0, rot, 0));
						(*players)[0]->getOrientation()->setScale(scale);

						tempSec->setOnFace((*players)[0]);
						(*players)[0]->YEET = ((float) (rand() % 629)) * 0.01f + 6.28f;
					}
				}
			}
			else
			{
				for (int i = 0; i < _OBJ->size() && !FOUND; i++)
				{
					if ((*_OBJ)[i]->getIdentity() == temp)
					{
						FOUND = true;
						Object* objCont = new Destructable;
						objCont->AttachObject((*_OBJ)[i]);

						if (temp == "GRASS_SPROUT")
						{
							objCont->setBezTimeLimit(1.f);
						}

						std::getline(theFile, temp);
						float xLoc = stof(temp);
						std::getline(theFile, temp);
						float yLoc = stof(temp);
						std::getline(theFile, temp);
						int rot = stoi(temp);
						std::getline(theFile, temp);
						float scale = stof(temp);

						objCont->setPosition(vec3(xLoc, 0.f, yLoc));
						objCont->setRotation(vec3(0, rot, 0));
						objCont->getOrientation()->setScale(scale);

						tempSec->setOnFace(objCont);
						objCont->YEET = ((float)(rand() % 629)) * 0.01f + 6.28f;
					}
				}
			}
		}
	}

}
