#pragma once
#include "Object.h"

class UIelement
{
public:
	UIelement();
	UIelement(Object* _OBJ);
	~UIelement();
private:
	Object* UIObj;
};