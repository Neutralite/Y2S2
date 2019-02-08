#pragma once
#include "Object.h"

class Destructable : public Object
{
public:
	Destructable();
	//Destructable(Object* _OBJ, std::string _NAME, int _MAX_HP);
	~Destructable();
	virtual void updatePhysics(float dt);
private:
};