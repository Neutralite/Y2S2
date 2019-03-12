#pragma once
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include <fstream>

class Text : public Transform
{
public:
	Text();
	~Text();

	bool init(std::string _font, std::string _material, std::string shader);
	void setMessage(std::string _message);

	void draw();
	void update(float dt);

	std::vector<vec3> tS;
	vec3 aS = vec3(1.f);
	std::vector<vec3> colorShift;

	vec3 baseColor = vec3(0.5f);
	std::vector<vec3> posOffset;
	float wordLength = 0.f;

	unsigned int messageSize();
protected:
	std::vector<vec3> individualPos;

	std::vector<vec4> XYWH;
	Material* material;
	Mesh* _M_QUAD;

	int tX, tY;
	std::string message;

	float totalUpdateCounter = 0.f;
	bool _isInit = false;
};

