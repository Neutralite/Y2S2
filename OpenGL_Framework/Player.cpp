#include "Player.h"
#include <iostream>
#include "Powerup.h"

Player::Player()
{
	TT = TransformType::TYPE_Player;
}

Player::~Player()
{
	delete Engine;
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
		getPhysicsBody()->setVelocityLimit(200.f);
		getPhysicsBody()->setAngularFriction(0.f);
		getPhysicsBody()->setAngularVelocityLimit(180.f);
		getPhysicsBody()->setMass(1.01f);

		maxHP = 100.f;
	}
	else if (PT == PLAYER_TYPE::BULLDOZER)
	{
		Engine->setFriction(90.f);
		Engine->setVelocityLimit(27.f);
		baseAcceleration = 155.f;
		steeringForce = 1000.f;
		Engine->setAngularFriction(0.f);
		Engine->setAngularVelocityLimit(220.f);

		getPhysicsBody()->setFriction(90.f);
		getPhysicsBody()->setVelocityLimit(200.f);
		getPhysicsBody()->setAngularFriction(0.f);
		getPhysicsBody()->setAngularVelocityLimit(220.f);
		getPhysicsBody()->setMass(1.21f);

		maxHP = 120.f;
	}
	else if (PT == PLAYER_TYPE::WRECKING_BALL)
	{
		Engine->setFriction(20.f);
		Engine->setVelocityLimit(35.f);
		baseAcceleration = 90.f;
		steeringForce = 600.f;
		Engine->setAngularFriction(0.f);
		Engine->setAngularVelocityLimit(130.f);

		getPhysicsBody()->setFriction(20.f);
		getPhysicsBody()->setVelocityLimit(200.f);
		getPhysicsBody()->setAngularFriction(0.f);
		getPhysicsBody()->setAngularVelocityLimit(130.f);
		getPhysicsBody()->setMass(1.32f);

		maxHP = 90.f;
	}
	else if (PT == PLAYER_TYPE::TANK)
	{
		Engine->setFriction(100.f);
		Engine->setVelocityLimit(25.f);
		baseAcceleration = 180.f;
		steeringForce = 1500.f;
		Engine->setAngularFriction(0.f);
		Engine->setAngularVelocityLimit(300.f);

		getPhysicsBody()->setFriction(100.f);
		getPhysicsBody()->setVelocityLimit(200.f);
		getPhysicsBody()->setAngularFriction(0.f);
		getPhysicsBody()->setAngularVelocityLimit(300.f);
		getPhysicsBody()->setMass(1.85f);

		maxHP = 150.f;
	}
	
	playerHP = maxHP;
	LERP_HP = maxHP;
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

	for (unsigned int i = 0; i < dynVels.size(); i++)
	{
		if (dot(dynVels[i] - getVelocity(), dynNorm[i]) > 0)
		{
			vec3 tNorm = normalize(dynNorm[i]);
			vec3 velPara2 = tNorm * dot(tNorm, dynVels[i]);
			//vec3 velPerp2 = dynVels[i] - velPara2;

			vec3 velPara1 = tNorm * dot(tNorm, getVelocity());
			vec3 velPerp1 = getVelocity() - velPara1;

			vec3 vFin = velPerp1 + velPara1 + (velPara2 - getVelocity()) * 2.f * dynMass[i] / (dynMass[i] + 1.f);

			getPhysicsBody()->setVelocity(vFin * 1.f);

			Engine->setVelocity(vec3(0.f));

			putBack = true;
		}
	}

	if (dot(pushAgainst, getVelocity()) < 0)
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
	dynVels.clear();
	dynMass.clear();
	dynNorm.clear();
}

void Player::draw()
{
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
			if (_GO->getPhysicsBody()->getHB()->dynamic)
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					vec3 centerToCollision = normalize(getPhysicsBody()->getHB()->closestPoint);
					vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);
					
					centerToCollision.y = 0.f;
					outward.y = 0.f;

					dynNorm.push_back(outward);
					dynVels.push_back(_GO->getVelocity());
					dynMass.push_back(1.25f);

					needsUpdate = true;
					//
				}
			}
			else if (_GO->TT == TYPE_Powerup)
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					_GO->needsUpdate = true;
					_GO->destroying = true;
					needsUpdate = true;
				}
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
					if (length(Engine->getVelocity()) * getPhysicsBody()->getMass() > Engine->getVelocityLimit() * ((float)_GO->destrPoints) * 0.1f && _GO->TT != TransformType::TYPE_Boundary)
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

//void Player::initiateDestruction(int destrType, vec3 directionOutwards)
//{
//}

void Player::DestructionSequence(float dt)
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

void Player::terminateEngine()
{
	getPhysicsBody()->setVelocity(vec3(0.f));
	Engine->setVelocity(vec3(0.f));
}
