#pragma once
#include "HITBOX2D.h"

class PhysicsBody
{
public:
	PhysicsBody();
	PhysicsBody(Hitbox2D* hb);
	PhysicsBody(Hitbox2D* hb, vec3 pos, vec3 angs);
	~PhysicsBody();
	void setToBase();
	void resetForMap();

	void update(float dt);

	Hitbox2D* collider;
	int heightOfBox;

	bool isEnabled = true;
	bool hitboxEnabled = true;

	vec3 acceleration;
	vec3 velocity;
	vec3 position;

	vec3 torque;
	vec3 angularVelocity;
	vec3 rotationAngles;

	vec3 direction;
	vec3 vDir;

	vec3 preVel;
	vec3 prePos;

	vec3 axisOfRotation;
	float rotationAmnt;

	float velocityLimit;
	float angVelocityLimit;
	float baseAcc;
	float baseTorque;
	bool goingForward = true;

	float friction;
	float drag;
};