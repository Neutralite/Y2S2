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

	if (attackTimer >= 0.f)
		attackTimer -= dt;
	else
		attackTimer = 0.f;

	//Insert stuff here to make sure the weapon is removed when the number of them is used up

	sendATTACK = false;
	if (mATTACK && attackTimer < 0.f)
		performAttack(dt);

	needsUpdate = false;

	direction = getLocalRot().forward();
}

void Player::performAttack(float dt)
{
	if (attack)
	{
		sendATTACK = true;
		attackTimer = attack->coolDownTime();
	}
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

	Engine->setAngularVelocity(vec3(0, steer * min(velRat, Engine->getAngularVelocityLimit()) * steeringMultiplier, 0));
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
	//if (getMesh() && !HIDE)
	//{
	//	//std::cout << "DRAWINGPLAYER" << std::endl;
	//	getShader()->bind();
	//	getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
	//	getShader()->sendUniform("bob", randomBob);
	//	if (length(lerpVel) > 0)
	//		getShader()->sendUniform("sway", -normalize(lerpVel) * 0.5f * (1.f - (float)pow(2, 0.1f * -length(lerpVel))));
	//	else
	//		getShader()->sendUniform("sway", vec3());
	//	//std::cout << getShader()->getName() << std::endl;
	//	int i = 0;
	//	for (Texture* texture : *(getTextures()))
	//	{
	//		texture->bind(i++);
	//	}
	//	getMesh()->draw();
	//	for (Texture* texture : *(getTextures()))
	//	{
	//		texture->unbind(--i);
	//	}
	//}

	if (getMesh() && getMaterial() && !HIDE)
	{
		getMaterial()->bind();
		getMaterial()->sendUniforms();
		getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		getShader()->sendUniform("bob", randomBob);
		if (length(lerpVel) > 0)
			getShader()->sendUniform("sway", -normalize(lerpVel) * 0.5f * (1.f - (float)pow(2, 0.1f * -length(lerpVel))));
		else
			getShader()->sendUniform("sway", vec3());
		int i = 0;
		for (Texture* texture : *(getTextures()))
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		getMesh()->draw();
		for (Texture* texture : *(getTextures()))
		{
			texture->unbind(--i);
		}
	}
}

void Player::attachWeapon(Weapon * W)
{
	attack = W;
	attackTimer = 0.f;
}

Weapon * Player::getWeapon()
{
	return attack;
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

void Player::doCollision(GameObject* _GO)
{
	if (_GO->getPhysicsBody()->getHB() && getPhysicsBody()->getHB() && !_GO->destroyed && !_GO->destroying)
	{
		if (_GO->getPhysicsBody()->getHB()->enabled && getPhysicsBody()->getHB()->enabled)
		{
			//std::cout << _GO->getName() << std::endl;
			if (_GO->getPhysicsBody()->getHB()->dynamic)
			{

			}
			else if (_GO->getPhysicsBody()->getHB()->grass)
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					if (length(getVelocity()) > 0.f)
					{
						vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);
						vec3 BP1 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
						vec3 BP2 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
						_GO->applySwing(BP1, BP2, 0.7f);
					}

					_GO->needsUpdate = true;
					needsUpdate = true;
				}
			}
			else
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					//std::cout << length(getPhysicsBody()->getVelocity()) - getPhysicsBody()->getVelocityLimit() << std::endl;
					if (length(Engine->getVelocity()) > Engine->getVelocityLimit() * ((float)_GO->destrPoints) * 0.1f && _GO->TT != TransformType::TYPE_Boundary)
					{
						_GO->initiateDestruction(0, normalize(getPhysicsBody()->getHB()->outDir), 0.5f, playerNumber);
					}
					else
					{
						vec3 centerToCollision = normalize(getPhysicsBody()->getHB()->closestPoint);
						vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);

						centerToCollision.y = 0.f;
						outward.y = 0.f;

						if (dot(outward, getVelocity()) < 0)
						{
							pushAgainst += outward;
						}

						if (dot(outward, getVelocity()) < 0.f)
						{
							vec3 BP2 = getVelocity() * getPhysicsBody()->getMass() * 0.05f;
							vec3 BP1 = (-getVelocity() + outward * 2 * dot(outward, getVelocity())) * getPhysicsBody()->getMass() * 0.05f;
							if (length(BP1) > length(_GO->swingPoint1) || _GO->swingTime <= 0.f)
							{
								_GO->applySwing(BP1, BP2, 1.f);
								//std::cout << "HELLO" << std::endl;
							}

							_GO->needsUpdate = true;
							needsUpdate = true;
						}
					}
				}
			}
		}
	}
}

void Player::initiateDestruction(int destrType, vec3 directionOutwards)
{
}

void Player::resetToInitials()
{
	//std::cout << "HIYA" << std::endl;
	POINT_TOTAL = 0;
	//LERP_TOTAL = 0;
	GameObject::resetToInitials();
	Engine->reset();
}
