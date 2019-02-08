#pragma once

#include <string>
#include "Jmath.h"

class Light
{
public:
	Light();
	Light(vec4 _DIR, vec4 _POS, vec4 _DSE, vec4 clq, vec4 col, std::string _name, int _TYPE);
	~Light();
	vec4 getDirection();
	vec4 getPosition();
	vec4 getDiffSpecExp();
	vec4 getCLQ();
	vec4 getColor();
	int getLightType();
	std::string getName();
	void setDirection(vec4 _D);
	void setPosition(vec4 _P);
	void setDiffSpecExp(vec4 _DSE);
	void lightIsPoint();
	void lightIsDirectional();
	void lightIsSpot();
	void setName(std::string _NAME);
	void setCLQ(vec4 _CLQ);
	void setColor(vec4 _COLOR);
private:
	vec4 direction;
	vec4 position;
	vec4 diffSpecExp;
	vec4 CLQ;
	vec4 color;
	int lightType = 0;
	std::string name;
};