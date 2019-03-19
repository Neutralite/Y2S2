#pragma once
#include "GameObject.h"

class Destructable : public GameObject
{
public:
	Destructable();
	~Destructable();

	void setBob(float bob);
	float getBob();
	void draw();
	void update(float dt);

	vec3 bezierPoint();
private:
	float randomBob;
};