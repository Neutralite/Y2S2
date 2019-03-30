#pragma once
#include "ResourceManager.h"
#include "BasePlate.h"
#include "Boundary.h"
#include "Destructable.h"
#include "Player.h"

class Field
{
public:
	Field();
	Field(std::string &fileName);
	~Field();

	std::vector<std::vector<BasePlate*>> grid;
	std::vector<std::vector<std::vector<GameObject*>>> fieldObjects;
	std::vector<Player*> players;
	std::vector<vec3> spawnPoints;
	std::vector<vec3> gridPoints;
	unsigned int height = 100, width = 100;
	float sectHeight = 6.f, sectWidth = 6.f;
	void loadFromFile(std::string &fileName, std::string &dir);
	void removeObj(int _X, int _Y, GameObject* _GO);
};