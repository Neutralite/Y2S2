#pragma once
#include "Weapon.h"

class Mine : public Weapon
{
public:
	Mine();
	~Mine();
	static bool weaponInit();

	float getDMG() { return 25.f; }

	static std::vector<wFrame> FRAMES;

	std::vector<wFrame>* getFL();

	unsigned int getDest();
	void otherUpdates(float dt);

	float frameWarp(float t);

	int destrType = 0;

	static float COOL_DOWN;
	float coolDownTime();
};