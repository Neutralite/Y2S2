#pragma once

#include "Jmath.h"
#include <vector>
#include <string>

class Hitbox2D
{
public:
	Hitbox2D();
	~Hitbox2D();
	void loadFromFile(std::string &fileName);
	bool collidesWith(Hitbox2D* h, mat4* ownWorldMat, mat4* otherWorldMat);

	float maxRad;
	vec3 outDir;
	int closestPoint;
	bool dynamic;
	bool enabled;
	bool grass = false;
	bool unbreakable = false;
	std::vector<vec3> points;
	std::vector<vec3> normals;
	std::string name;
};