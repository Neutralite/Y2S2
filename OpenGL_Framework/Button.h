#pragma once
#include "UIelement.h"

class Button : public UIelement
{
public:
	Button();
	Button(float _X, float _Y, float _W, float _H);
	Button(vec2 bottomLeftCorner, vec2 widthAndHeight);
	Button(vec4 cornerAndDimensions);
	Button(float _X, float _Y, float _W, float _H, Object* _BTTN);
	Button(vec2 bottomLeftCorner, vec2 widthAndHeight, Object* _BTTN);
	Button(vec4 cornerAndDimensions, Object* _BTTN);
	~Button();

	void setBottomLeft(float _X, float _Y);
	void setDimensions(float _W, float _H);
	void setBottomLeft(vec2 _XY);
	void setDimensions(vec2 _WH);
	void setCoors(vec4 _COORDINATES);

	void setObject(Object* _OBJ);
	Object* getObject();

	bool contains(vec2 mousePos);
private:
	vec4 buttonCoors;
	Object* buttonObj;
};