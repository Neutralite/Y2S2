#include "Field.h"
#include <fstream>
#include <iostream>
#include "ExternalPlayerData.h"

Field::Field()
{
}

Field::Field(std::string & fileName)
{
	grid.clear();
	players.clear();
	spawnPoints.clear();
	fieldObjects.clear();

	for (unsigned int i = 0; i < width; i++)
	{
		std::vector<BasePlate*> sub;
		std::vector<std::vector<GameObject*>> SUB;
		for (unsigned int j = 0; j < height; j++)
		{
			BasePlate* sec = nullptr;
			std::vector<GameObject*> SEC;
			sub.push_back(sec);
			SUB.push_back(SEC);
		}
		fieldObjects.push_back(SUB);
		grid.push_back(sub);
	}
	std::string temp = "../assets/Map/";
	loadFromFile(fileName, temp);
}

Field::~Field()
{
}

void Field::loadFromFile(std::string & fileName, std::string &dir)
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
			loadFromFile(temp, blop);
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

		BasePlate* BASE = ResourceManager::getCloneOfBasePlate(PlateName);

		BASE->setLocalPos(vec3(sectWidth * gridSpotX, 0.f, sectHeight * gridSpotY));
		BASE->setLocalRot(vec3(0, (float)(tileRot * 90), 0));

		Light* VECT_LIT;// = rm::getCloneOfLight("THE_1000_LIGHTS");
		for (int i = 0; i < 5; i++)
		{
			VECT_LIT = rm::getCloneOfLight("THE_1000_LIGHTS");
			VECT_LIT->setLocalPos(vec3((rand() % 51 - 25) / 10.f, 0.01f, (rand() % 51 - 25) / 10.f));
			BASE->addChild(VECT_LIT);
		}

		grid[gridSpotX][gridSpotY] = BASE;

		while (std::getline(theFile, temp))
		{
			if (temp == "PLACEHOLDER_PLAYER")
			{
				std::getline(theFile, temp);
				float xLoc = stof(temp);
				std::getline(theFile, temp);
				float yLoc = stof(temp);
				std::getline(theFile, temp);
				int rot = stoi(temp);
				std::getline(theFile, temp);

				spawnPoints.push_back(vec3(xLoc, 0, yLoc));
				gridPoints.push_back(vec3((float)gridSpotX, (float)gridSpotY, (float)rot));
			}
			else
			{
				Destructable* DESTR = ResourceManager::getCloneOfDestructable(temp);

				std::getline(theFile, temp);
				float xLoc = stof(temp);
				std::getline(theFile, temp);
				float yLoc = stof(temp);
				std::getline(theFile, temp);
				int rot = stoi(temp);
				std::getline(theFile, temp);
				float scale = stof(temp);

				DESTR->setLocalPos(vec3(xLoc, 0.f, yLoc));
				DESTR->setLocalRot(vec3(0, (float)rot, 0));
				DESTR->setScale(scale);

				fieldObjects[gridSpotX][gridSpotY].push_back(DESTR);
				DESTR->setBob(((float)(rand() % 629)) * 0.01f + 6.28f);

				DESTR->mapX = gridSpotX;
				DESTR->mapY = gridSpotY;

				DESTR->setInitials(vec3(xLoc, 0.f, yLoc), vec3(0, (float)rot, 0), vec3(scale));
				
				DESTR->hasInitial = true;
			}
		}
	}
}

void Field::removeObj(int _X, int _Y, GameObject * _GO)
{
	for (int i = fieldObjects[_X][_Y].size() - 1; i >= 0; --i)
		if (fieldObjects[_X][_Y][i] == _GO)
		{
			//std::cout << "I AM HERE" << std::endl;
			fieldObjects[_X][_Y].erase(fieldObjects[_X][_Y].begin() + i);
		}
}
