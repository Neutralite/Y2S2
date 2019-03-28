#pragma once
#include "Weapon.h"

class Axe : public Weapon
{
public:
	Axe();
	~Axe();
	static bool weaponInit();

	static std::vector<wFrame> FRAMES;

	std::vector<wFrame>* getFL();

	unsigned int getDest();
	void otherUpdates(float dt);

	float frameWarp(float t);

	int destrType = 0;

	static float COOL_DOWN;
	float coolDownTime();
};