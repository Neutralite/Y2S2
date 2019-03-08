#pragma once

#include <gmtk/gmtk.h>
#include <vector>
#include <string>

class Hitbox2D
{
public:
	Hitbox2D();
	~Hitbox2D();
	void loadFromFile(std::string &fileName);
	bool collidesWith(Hitbox2D* h, mat4 ownWorldMat, mat4 otherWorldMat, bool showRes = false);

	std::string getName();
	void setName(std::string _NAME);

	float maxRad;
	vec3 outDir;
	vec3 closestPoint;
	bool dynamic;
	bool enabled;
	bool grass = false;
	bool unbreakable = false;
	std::vector<vec3> points;
	std::vector<vec3> normals;
	std::string name;
};