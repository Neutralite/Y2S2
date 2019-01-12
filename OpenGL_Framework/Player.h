#pragma once
#include "Object.h"
#include "PhysicsBody.h"

class Player : public Object
{
public:
	Player();
	~Player();
	void updatePhysics(float dt);
	void giveInput(char A, bool B);
	void dealWithInputs(float dt);
	void setCam(Camera* cam);
	vec3* getTilt();
	Camera* getCam();

	bool keys[256] = { false };
	bool singleBuffer = true;
	vec3 vSub;
	vec3 vChase = vec3(0, 0, 0);
	vec3 vApart;
	Camera* pCam, UI;
};