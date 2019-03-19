#pragma once
#include "Weapon.h"

class Hammer : public Weapon
{
public:
	Hammer();
	~Hammer();
	bool weaponInit();

	static std::vector<wFrame> FRAMES;

	bool tailoredCollision(GameObject* _GO);

	wFrame* getFrameBefore(float t);
	wFrame* getFrameAt(float t);
	wFrame* getFrameAfter(float t);
	wFrame* getFrame2After(float t);

	unsigned int getDest();
	void otherUpdates(float dt);

	int destrType = 0;
};