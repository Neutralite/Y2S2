#include "clickBox.h"

clickBox::clickBox()
{
	coors = vec4(0, 0, 0, 0);
}

clickBox::clickBox(vec4 _coors)
{
	coors = _coors;
}

clickBox::clickBox(vec2 XAY, vec2 WAH)
{
	coors = vec4(XAY, WAH);
}

clickBox::clickBox(float _x, float _y, float _w, float _h)
{
	coors = vec4(_x, _y, _w, _h);
}

void clickBox::setX(float X)
{
	coors.x = X;
}

void clickBox::setY(float Y)
{
	coors.y = Y;
}

void clickBox::setWidth(float W)
{
	coors.z = W;
}

void clickBox::setHeight(float H)
{
	coors.w = H;
}

void clickBox::setPos(vec2 XAY)
{
	coors.x = XAY.x;
	coors.y = XAY.y;
}

void clickBox::setPos(float X, float Y)
{
	coors.x = X;
	coors.y = Y;
}

void clickBox::setDim(vec2 WAH)
{
	coors.z = WAH.x;
	coors.w = WAH.y;
}

void clickBox::setDim(float W, float H)
{
	coors.z = W;
	coors.w = H;
}

void clickBox::setCoord(vec4 _coors)
{
	coors = _coors;
}

void clickBox::setCoord(vec2 XAY, vec2 WAH)
{
	coors = vec4(XAY, WAH);
}

void clickBox::setCoord(float X, float Y, float W, float H)
{
	coors = vec4(X, Y, W, H);
}

vec4 clickBox::getCoord()
{
	return coors;
}

vec2 clickBox::getWidthAndHeight()
{
	return vec2(coors.z, coors.w);
}

vec2 clickBox::getXandY()
{
	return vec2(coors.x, coors.y);
}

float clickBox::getX()
{
	return coors.x;
}

float clickBox::getY()
{
	return coors.y;
}

float clickBox::getW()
{
	return coors.z;
}

float clickBox::getH()
{
	return coors.w;
}

bool clickBox::contains(vec2 point)
{
	return point.x > coors.x - coors.z * 0.5f && point.x < coors.x + coors.z * 0.5f && point.y > coors.y - coors.w * 0.5f && point.y < coors.y + coors.w * 0.5f;
}

bool clickBox::contains(float X, float Y)
{
	return X > coors.x - coors.z * 0.5f && X < coors.x + coors.z * 0.5f && Y > coors.y - coors.w * 0.5f && Y < coors.y + coors.w * 0.5f;
}
