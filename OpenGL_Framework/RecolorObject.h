#pragma once
#include "GameObject.h"

class RecolorObject : public GameObject
{
public:
	RecolorObject();
	~RecolorObject();

	void draw();

	vec3 objectColor;
};