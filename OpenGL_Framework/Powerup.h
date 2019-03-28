#pragma once
#include "GameObject.h"

class Powerup : public GameObject
{
public:
	Powerup();
	~Powerup();

	enum pType
	{
		MINE,
		HAMMER,
		AXE
	};
	void setPower(pType PT);

	void draw();
	void update(float dt);

	vec3 bezierPoint();

	float randomColorOffset = 0.f;
	float timeAccum = 0.f;
	pType _P;
};