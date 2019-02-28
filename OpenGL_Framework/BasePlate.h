#pragma once
#include "GameObject.h"

class BasePlate : public GameObject
{
public:
	BasePlate();
	~BasePlate();

	void setBoundingRadius(float boundingRadius);
	float getBoundingRadius();
private:
	float _BR;
};