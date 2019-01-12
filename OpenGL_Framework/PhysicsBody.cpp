#include "PhysicsBody.h"
#include <iostream>

PhysicsBody::PhysicsBody()
{
	setToBase();
}

PhysicsBody::PhysicsBody(Hitbox2D* hb)
{
	setToBase();
	collider = hb;
}

PhysicsBody::PhysicsBody(Hitbox2D* hb, vec3 pos, vec3 angs)
{
	setToBase();
	collider = hb;
	rotationAngles = angs;
	position = pos;
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::setToBase()
{
	collider = nullptr;
	rotationAngles = vec3(0, 0, 0);
	angularVelocity = vec3(0, 0, 0);
	torque = vec3(0, 0, 0);
	position = vec3(0, 0, 0);
	velocity = vec3(0, 0, 0);
	acceleration = vec3(0, 0, 0);
	direction = vec3(0, 0, -1);
	vDir = direction;

	friction = 0.f;
	drag = 0.f;

	velocityLimit = 0.f;
	angVelocityLimit = 0.f;
	baseAcc = 0.f;
	baseTorque = 0.f;

	preVel = vec3(0, 0, 0);
	prePos = vec3(0, 0, 0);

	axisOfRotation = vec3(0, 0, 1);
	rotationAmnt = 0;
}

void PhysicsBody::resetForMap()
{
	position = vec3(0, 0, 0);
	velocity = vec3(0, 0, 0);
	acceleration = vec3(0, 0, 0);

	rotationAngles = vec3(0, 0, 0);
	angularVelocity = vec3(0, 0, 0);
	torque = vec3(0, 0, 0);

	axisOfRotation = vec3(0, 0, 1);
	rotationAmnt = 0;

	preVel = vec3(0, 0, 0);
	prePos = vec3(0, 0, 0);
}

void PhysicsBody::update(float dt)
{
	preVel = velocity;
	prePos = position;


	//std::cout << acceleration.Length() << std::endl;

	velocity += acceleration * dt;

	if (velocity.LengthSquared() > 0 && friction > 0)
	{
		if (velocity.Length() > friction * dt)
		{
			velocity -= velocity.GetNormalized() * friction * dt;
		}
		else
		{
			velocity = vec3(0, 0, 0);
		}
	}
	
	if (velocity.Length() > velocityLimit)
		velocity = velocity.GetNormalized() * velocityLimit;

	//std::cout << velocity.x << ", " << velocity.z << std::endl;

	position += velocity * dt;

	angularVelocity += torque * dt;

	if (angularVelocity.LengthSquared() > 0 && friction > 0)
	{
		if (angularVelocity.Length() > friction * dt)
		{
			angularVelocity -= angularVelocity.GetNormalized() * friction * dt;
		}
		else
		{
			angularVelocity = vec3(0, 0, 0);
		}
	}

	if (angularVelocity.Length() > angVelocityLimit)
		angularVelocity = angularVelocity.GetNormalized() * angVelocityLimit;

	rotationAngles += angularVelocity * dt;
	if (angularVelocity.Length() * dt > 0)
	{
		axisOfRotation = angularVelocity.GetNormalized();
		rotationAmnt = angularVelocity.Length() * dt;
	}

	if (velocity.LengthSquared() > 0)
	{
		direction = velocity.GetNormalized();
		if (Dot(vDir, direction) < 0)
		{
			vDir = -direction;
		}
		else
		{
			vDir = direction;
		}
	}

	//std::cout << velocity.x << ", " << velocity.z << "\n-----" << std::endl;
}
