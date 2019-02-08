#include "LIGHT_SOURCE.h"

Light::Light()
{
	direction = vec4(0, 0, 1, 0);
	position = vec4(0, 0, 0, 1);
	diffSpecExp = vec4(0, 0, 0, 0);
	CLQ = vec4(0, 0, 0, 0);
	color = vec4(1, 1, 1, 1);
	name = "_NULL";
}

Light::Light(vec4 _DIR, vec4 _POS, vec4 _DSE, vec4 clq, vec4 col, std::string _name, int _TYPE)
{
	direction = _DIR;
	position = _POS;
	diffSpecExp = _DSE;
	CLQ = clq;
	color = col;
	name = _name;
	lightType = _TYPE;
}

Light::~Light()
{
}

vec4 Light::getDirection()
{
	return direction;
}

vec4 Light::getPosition()
{
	return position;
}

vec4 Light::getDiffSpecExp()
{
	return diffSpecExp;
}

vec4 Light::getCLQ()
{
	return CLQ;
}

vec4 Light::getColor()
{
	return color;
}

int Light::getLightType()
{
	return lightType;
}

std::string Light::getName()
{
	return name;
}

void Light::setDirection(vec4 _D)
{
	direction = _D;
}

void Light::setPosition(vec4 _P)
{
	position = _P;
}

void Light::setDiffSpecExp(vec4 _DSE)
{
	diffSpecExp = _DSE;
}

void Light::lightIsPoint()
{
	lightType = 0;
}

void Light::lightIsDirectional()
{
	lightType = 1;
}

void Light::lightIsSpot()
{
	lightType = 2;
}

void Light::setName(std::string _NAME)
{
	name = _NAME;
}

void Light::setCLQ(vec4 _CLQ)
{
	CLQ = _CLQ;
}

void Light::setColor(vec4 _COLOR)
{
	color = _COLOR;
}
