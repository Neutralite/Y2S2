#include "Button.h"

Button::Button()
{
	buttonCoors = vec4(0, 0, 0, 0);
	buttonObj = nullptr;
}

Button::Button(float _X, float _Y, float _W, float _H)
{
	buttonCoors = vec4(_X, _X, _W, _H);
	buttonObj = nullptr;
}

Button::Button(vec2 bottomLeftCorner, vec2 widthAndHeight)
{
	buttonCoors = vec4(bottomLeftCorner.x, bottomLeftCorner.y, widthAndHeight.x, widthAndHeight.y);
	buttonObj = nullptr;
}

Button::Button(vec4 cornerAndDimensions)
{
	buttonCoors = cornerAndDimensions;
	buttonObj = nullptr;
}

Button::Button(float _X, float _Y, float _W, float _H, Object * _BTTN)
{
	buttonCoors = vec4(_X, _X, _W, _H);
	buttonObj = _BTTN;
}

Button::Button(vec2 bottomLeftCorner, vec2 widthAndHeight, Object * _BTTN)
{
	buttonCoors = vec4(bottomLeftCorner.x, bottomLeftCorner.y, widthAndHeight.x, widthAndHeight.y);
	buttonObj = _BTTN;
}

Button::Button(vec4 cornerAndDimensions, Object * _BTTN)
{
	buttonCoors = cornerAndDimensions;
	buttonObj = _BTTN;
}

Button::~Button()
{
}

void Button::setBottomLeft(float _X, float _Y)
{
	buttonCoors.x = _X;
	buttonCoors.y = _Y;
}

void Button::setDimensions(float _W, float _H)
{
	buttonCoors.z = _W;
	buttonCoors.w = _H;
}

void Button::setBottomLeft(vec2 _XY)
{
	buttonCoors.x = _XY.x;
	buttonCoors.y = _XY.y;
}

void Button::setDimensions(vec2 _WH)
{
	buttonCoors.z = _WH.x;
	buttonCoors.w = _WH.y;
}

void Button::setCoors(vec4 _COORDINATES)
{
	buttonCoors = _COORDINATES;
}

void Button::setObject(Object * _OBJ)
{
	buttonObj = _OBJ;
}

Object * Button::getObject()
{
	return buttonObj;
}

bool Button::contains(vec2 mousePos)
{
	return (mousePos.x >= buttonCoors.x && mousePos.y <= buttonCoors.x + buttonCoors.z && mousePos.y >= buttonCoors.y && mousePos.y <= buttonCoors.y + buttonCoors.w);
}
