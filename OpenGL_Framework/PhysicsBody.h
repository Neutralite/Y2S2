#pragma once
#include "HITBOX2D.h"

class PhysicsBody
{
public:
	PhysicsBody(Hitbox2D *_HB = nullptr, float _MASS = 1.f);
	~PhysicsBody();

	void reset();
	void addForce(vec3 force);
	void update(float dt);

	vec3 getAcceleration();
	void setAcceleration(vec3 ACC);
	vec3 getVelocity();
	void setVelocity(vec3 VEL);
	vec3 getPosition();

	vec3 getAngularAcceleration();
	void setAngularAcceleration(vec3 TOR);
	vec3 getAngularVelocity();
	void setAngularVelocity(vec3 ANG);
	vec3 getRotationAngles();

	float getMass();
	void setMass(float _MASS);

	Hitbox2D* getHB();
	void setHB(Hitbox2D* _HB);

	void setFriction(float FRIC);
	float getFriction();
	void setAngularFriction(float FRIC);
	float getAngularFriction();

	void setVelocityLimit(float LIM);
	float getVelocityLimit();
	void setAngularVelocityLimit(float LIM);
	float getAngularVelocityLimit();
private:
	vec3 acceleration;
	vec3 velocity;
	vec3 position;
	float velocityLimit;
	float mass;

	vec3 angularAcceleration;
	vec3 angularVelocity;
	vec3 rotationAngles;
	float angularVelocityLimit;

	float friction;
	float angularFriction;

	Hitbox2D* HB;
};