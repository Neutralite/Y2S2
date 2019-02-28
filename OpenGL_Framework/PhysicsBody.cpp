#include "PhysicsBody.h"
#include <iostream>

PhysicsBody::PhysicsBody(Hitbox2D * _HB, float _MASS)
{
	HB = _HB;
	mass = _MASS;
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::reset()
{
	acceleration = vec3(0, 0, 0);
	velocity = vec3(0, 0, 0);
	position = vec3(0, 0, 0);
	
	angularAcceleration = vec3(0, 0, 0);
	angularVelocity = vec3(0, 0, 0);
	rotationAngles = vec3(0, 0, 0);
}

void PhysicsBody::addForce(vec3 force)
{
	acceleration += force / mass;
}

void PhysicsBody::update(float dt)
{
	position = vec3(0, 0, 0);
	velocity += acceleration * dt;

	if (length(velocity) - friction / mass * dt <= 0)
		velocity = vec3(0, 0, 0);
	else
		velocity -= normalize(velocity) * friction / mass * dt;

	if (length(velocity) > velocityLimit)
		velocity = normalize(velocity) * velocityLimit;

	position += velocity * dt;
	acceleration = vec3(0, 0, 0);

	rotationAngles = vec3(0, 0, 0);
	angularVelocity += angularAcceleration * dt;

	if (length(angularVelocity) - angularFriction / mass * dt <= 0)
		angularVelocity = vec3(0, 0, 0);
	else
		angularVelocity -= normalize(angularVelocity) * angularFriction * dt;

	if (length(angularVelocity) > angularVelocityLimit)
		angularVelocity = normalize(angularVelocity) * angularVelocityLimit;

	rotationAngles += angularVelocity * dt;
	angularAcceleration = vec3(0, 0, 0);
}

vec3 PhysicsBody::getAcceleration()
{
	return acceleration;
}

void PhysicsBody::setAcceleration(vec3 ACC)
{
	acceleration = ACC;
}

vec3 PhysicsBody::getVelocity()
{
	return velocity;
}

void PhysicsBody::setVelocity(vec3 VEL)
{
	velocity = VEL;
}

vec3 PhysicsBody::getPosition()
{
	return position;
}

vec3 PhysicsBody::getAngularAcceleration()
{
	return angularAcceleration;
}

void PhysicsBody::setAngularAcceleration(vec3 TOR)
{
	angularAcceleration = TOR;
}

vec3 PhysicsBody::getAngularVelocity()
{
	return angularVelocity;
}

void PhysicsBody::setAngularVelocity(vec3 ANG)
{
	angularVelocity = ANG;
}

vec3 PhysicsBody::getRotationAngles()
{
	return rotationAngles;
}

float PhysicsBody::getMass()
{
	return mass;
}

void PhysicsBody::setMass(float _MASS)
{
	mass = _MASS;
}

Hitbox2D * PhysicsBody::getHB()
{
	return HB;
}

void PhysicsBody::setHB(Hitbox2D * _HB)
{
	HB = _HB;
}

void PhysicsBody::setFriction(float FRIC)
{
	friction = FRIC;
}

float PhysicsBody::getFriction()
{
	return friction;
}

void PhysicsBody::setAngularFriction(float FRIC)
{
	angularFriction = FRIC;
}

float PhysicsBody::getAngularFriction()
{
	return angularFriction;
}

void PhysicsBody::setVelocityLimit(float LIM)
{
	velocityLimit = LIM;
}

float PhysicsBody::getVelocityLimit()
{
	return velocityLimit;
}

void PhysicsBody::setAngularVelocityLimit(float LIM)
{
	angularVelocityLimit = LIM;
}

float PhysicsBody::getAngularVelocityLimit()
{
	return angularVelocityLimit;
}
