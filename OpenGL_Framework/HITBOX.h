#pragma once

#include "Jmath.h"
#include <vector>
#include <string>

class Hitbox
{
public:
	Hitbox();
	~Hitbox();
	void loadFromFile(std::string &fileName);
	
	bool collidesWith(Hitbox* HB, mat4 ownWorldMat, mat4 otherWorldMat);
	vec3 getMedianOfCollision();
	vec3 getRepulsionVector();

	void setStatic(bool b);
	bool isStatic();
	void setEnabled(bool b);
	bool isEnabled();

	vec3 repulsionVector;
	vec3 repulsionMedian;
	std::vector<vec3> boxPoints;
	std::vector<std::vector<unsigned int>> faces;
	std::vector<std::vector<unsigned int>> lines;
	std::vector<vec3> faceNormals;
	std::string name;
	bool stationary = true;
	bool enabled = true;
};