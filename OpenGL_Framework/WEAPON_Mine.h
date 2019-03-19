#pragma once
#include "Weapon.h"

class Mine : public Weapon
{
public:
	Mine();
	~Mine();
	static bool weaponInit();

	static std::vector<wFrame> FRAMES;

	//bool tailoredCollision(GameObject* _GO);

	wFrame* getFrameBefore(float t);
	wFrame* getFrameAt(float t);
	wFrame* getFrameAfter(float t);
	wFrame* getFrame2After(float t);

	unsigned int getDest();
	void otherUpdates(float dt);

	float frameWarp(float t);

	int destrType = 0;

	static float COOL_DOWN;
	float coolDownTime();
};