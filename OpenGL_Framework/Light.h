#pragma once
#include "Transform.h"
#include "UniformBuffer.h"
#include "Mesh.h"

class Light : public Transform
{
public:
	enum LightType
	{
		Directional,
		Point,
		Spotlight
	};
	
	vec4 color;
	vec4 position;
	vec4 direction;
	
	float constantAtten;
	float linearAtten;
	float quadAtten;
	float radius;
	float intensity = 1.f;

	LightType type;

	Light();
	static void init();

	void update(float dt);
	float calculateRadius();

	static UniformBuffer _UBO;

	Mesh* LIGHT_MESH;
};