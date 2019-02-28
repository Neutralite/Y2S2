#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	enum PLAYER_IN
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		ATTACK
	};

	enum PLAYER_TYPE
	{
		TRUCK
	};

	Player();
	~Player();
	
	void playerInit(PLAYER_TYPE PT);

	void setBob(float bob);
	void sendInput(bool type, PLAYER_IN pin);
	float getBob();
	void update(float dt);
	void updatePhysics(float dt);
	void draw();

	vec3 getVelocity();
	vec3 getAcceleration();
	vec3 getAngularVelocity();
	vec3 getAngularPosition();

	float Yangle = 0.f;
private:

	bool mUP = false;
	bool mDOWN = false;
	bool mLEFT = false;
	bool mRIGHT = false;
	bool mATTACK = false;

	PhysicsBody *Engine;
	vec3 direction;
	vec3 right;
	vec3 catchUpVel;
	vec3 lerpVel;

	float randomBob;
	float baseAcceleration;
	float steeringForce;
};