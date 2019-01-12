#pragma once
#include "Jmath.h"

class clickBox
{
public:
	clickBox();
	clickBox(vec4 _coors);
	clickBox(vec2 XAY, vec2 WAH);
	clickBox(float _x, float _y, float _w, float _h);

	void setX(float X);
	void setY(float Y);
	void setWidth(float W);
	void setHeight(float H);

	void setPos(vec2 XAY);
	void setPos(float X, float Y);
	void setDim(vec2 WAH);
	void setDim(float W, float H);

	void setCoord(vec4 _coors);
	void setCoord(vec2 XAY, vec2 WAH);
	void setCoord(float X, float Y, float W, float H);

	vec4 getCoord();
	vec2 getWidthAndHeight();
	vec2 getXandY();
	float getX();
	float getY();
	float getW();
	float getH();


	bool contains(vec2 point);
	bool contains(float X, float Y);
private:
	vec4 coors;
};
