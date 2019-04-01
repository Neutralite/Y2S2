#pragma once
#include "Weapon.h"

class Hammer : public Weapon
{
public:
	Hammer();
	~Hammer();
	static bool weaponInit();

	float getDMG() { return 35.f; }

	static std::vector<wFrame> FRAMES;

	std::vector<wFrame>* getFL();

	unsigned int getDest();
	void otherUpdates(float dt);

	float frameWarp(float t);

	int destrType = 1;

	static float COOL_DOWN;
	float coolDownTime();
};