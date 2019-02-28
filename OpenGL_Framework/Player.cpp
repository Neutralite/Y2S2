#include "Player.h"
#include <iostream>

Player::Player()
{
	TT = TransformType::TYPE_Player;
}

Player::~Player()
{
}

void Player::playerInit(PLAYER_TYPE PT)
{
	direction = getLocalRot().forward();
	right = getLocalRot().right();

	//std::cout << direction << std::endl;

	Yangle = 0.f;
	Engine = new PhysicsBody();
	if (PT == PLAYER_TYPE::TRUCK)
	{
		Engine->setFriction(40.f);
		Engine->setVelocityLimit(30.f);
		baseAcceleration = 100.f;
		steeringForce = 720.f;
		Engine->setAngularFriction(0.f);
		Engine->setAngularVelocityLimit(180.f);

		getPhysicsBody()->setFriction(40.f);
		getPhysicsBody()->setVelocityLimit(30.f);
		getPhysicsBody()->setAngularFriction(0.f);
		getPhysicsBody()->setAngularVelocityLimit(180.f);
	}


	
	//std::cout << Engine << std::endl;
}

void Player::setBob(float bob)
{
	randomBob = bob;
}

void Player::sendInput(bool type, PLAYER_IN pin)
{
	switch (pin)
	{
	case PLAYER_IN::UP:
		mUP = type;
		break;
	case PLAYER_IN::DOWN:
		mDOWN = type;
		break;
	case PLAYER_IN::LEFT:
		mLEFT = type;
		break;
	case PLAYER_IN::RIGHT:
		mRIGHT = type;
		break;
	case PLAYER_IN::ATTACK:
		mATTACK = type;
		break;
	}
}

float Player::getBob()
{
	return randomBob;
}

void Player::update(float dt)
{
	if (Engine)
		updatePhysics(dt);
	Transform::update(dt);

	needsUpdate = false;

	direction = getLocalRot().forward();
}

void Player::updatePhysics(float dt)
{
	vec3 move = vec3(0, 0, 0);

	Yangle = getLocalEuler().y;

	//direction = getLocalRot().forward();

	float steer = 0;
	if (mUP)
		move -= direction;
	if (mLEFT)
		steer += steeringForce;
	if (mDOWN)
		move += direction;
	if (mRIGHT)
		steer -= steeringForce;

	if (length(move) > 0)
	{
		move = normalize(move) * baseAcceleration;
		Engine->addForce(move);
	}

	if (dot(Engine->getVelocity(), direction) > 0)
		steer *= -1.f;

	float velRat = pow(length(Engine->getVelocity()) / Engine->getVelocityLimit(), 2);

	Engine->setAngularVelocity(vec3(0, steer * velRat, 0));
	Engine->setVelocity(mat3::rotatey(degrees(Engine->getRotationAngles().y)) * Engine->getVelocity());
	
	bool putBack = false;

	if (length(pushAgainst) > 0)
	{
		pushAgainst = normalize(pushAgainst);
		getPhysicsBody()->setVelocity((getVelocity() - (pushAgainst * 2.f * dot(getVelocity(), pushAgainst))) * 0.6f);
		Engine->setVelocity(vec3(0.f));
		putBack = true;
	}

	Engine->update(dt);
	getPhysicsBody()->update(dt);

	catchUpVel = lerp(catchUpVel, Engine->getVelocity() + getPhysicsBody()->getVelocity(), 1.f - pow(0.9f, 60.f * dt));
	lerpVel = Engine->getVelocity() + getPhysicsBody()->getVelocity() - catchUpVel;

	if (putBack)
	{
		setLocalPos(getLocalPos() + getPhysicsBody()->getPosition());
	}
	else
	{
		setLocalPos(getLocalPos() + getPhysicsBody()->getPosition() + Engine->getPosition());
	}
	setLocalRotY(getLocalEuler().y + getPhysicsBody()->getRotationAngles().y + Engine->getRotationAngles().y);

	pushAgainst = vec3(0.f);
}

void Player::draw()
{
	if (getMesh())
	{
		//std::cout << "DRAWINGPLAYER" << std::endl;
		getShader()->bind();
		getShader()->sendUniform("uModel", getLocalToWorld());
		getShader()->sendUniform("bob", randomBob);
		if (length(lerpVel) > 0)
			getShader()->sendUniform("sway", -normalize(lerpVel) * 0.5f * (1.f - (float)pow(2, 0.1f * -length(lerpVel))));
		else
			getShader()->sendUniform("sway", vec3());
		//std::cout << getShader()->getName() << std::endl;
		int i = 0;
		for (Texture* texture : *(getTextures()))
		{
			texture->bind(i++);
		}
		getMesh()->draw();
		for (Texture* texture : *(getTextures()))
		{
			texture->unbind(--i);
		}
	}
}

vec3 Player::getVelocity()
{
	return getPhysicsBody()->getVelocity() + Engine->getVelocity();
}

vec3 Player::getAcceleration()
{
	return getPhysicsBody()->getAcceleration() + Engine->getAcceleration();
}

vec3 Player::getAngularVelocity()
{
	return getPhysicsBody()->getAngularVelocity() + Engine->getAngularVelocity();
}

vec3 Player::getAngularPosition()
{
	return getPhysicsBody()->getAngularAcceleration() + Engine->getAngularAcceleration();
}
