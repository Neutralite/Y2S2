#pragma once
#include <gmtk/gmtk.h>

class oFrame
{
public:
	vec3 position;
	vec3 rotationAngles;
	vec3 scale;
	float duration; //How long it happens for
	float timeOf; //When it happens

	enum MutationType
	{
		CATMULL,
		LINEAR
	};

	MutationType MT;

	float tFunc(float t);
};
