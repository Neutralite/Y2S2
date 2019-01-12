#include "Player.h"
#include <fstream>
#include <iostream>

Player::Player()
{
}

Player::~Player()
{
}

void Player::updatePhysics(float dt)
{

	vChase = LP::LERP(vChase, getPhysicsBody()->velocity, 0.1f);
	vApart = getPhysicsBody()->velocity - vChase;
	vSub = getPhysicsBody()->vDir;
	dealWithInputs(dt);
	getPhysicsBody()->update(dt);

	getPhysicsBody()->position.y = 0;
	getPhysicsBody()->velocity.y = 0;
	TranslateBy(getPhysicsBody()->position);
	RotateBy(getPhysicsBody()->rotationAngles);
	getPhysicsBody()->acceleration *= 0;
	getPhysicsBody()->position *= 0;
	getPhysicsBody()->rotationAngles *= 0;
	float DD = Dot(getPhysicsBody()->vDir, vec3(0, 0, -1));
	float CD = ToDegrees(acos(DD));
	if (DD < 1.f && DD > -1.f)
	{
		CD *= -Cross(getPhysicsBody()->vDir, vec3(0, 0, -1)).GetNormalized().y;
	}

	//if (!(getPhysicsBody()->goingForward))
	//{
	//	CD = CD + 180;
	//}
	//CD = fmod(CD, 360.f);

	getOrientation()->setRotationAngleY(CD);
}

void Player::giveInput(char A, bool B)
{
	//if (B == true)
	//{
	//	vec3 acc = vec3(0, 0, 0);
	//	if (A == 'U')
	//	{
	//		acc.z -= 1.f;
	//	}
	//	else if (A == 'D')
	//	{
	//		acc.z += 1.f;
	//	}
	//	else if (A == 'L')
	//	{
	//		acc.x -= 1.f;
	//	}
	//	else if (A == 'R')
	//	{
	//		acc.x += 1.f;
	//	}
	//
	//	if (acc.LengthSquared() > 0)
	//	{
	//		acc = acc.GetNormalized() * getPhysicsBody()->baseAcc;
	//	}
	//	
	//	getPhysicsBody()->acceleration = acc;
	//}
	keys[A - 1] = B;
}

void Player::dealWithInputs(float dt)
{
	vec3 acc = vec3(0, 0, 0);
	vec3 trueAcc = acc;
	float maxSteer = 5000.f;

	float air = ToRadians(getOrientation()->getRotationAngleY());

	if (keys['U' - 1])
	{
		acc.z -= 1.f * cos(air);
		acc.x -= 1.f * sin(air);
	}
	if (keys['D' - 1])
	{
		acc.z += 1.f * cos(air);
		acc.x += 1.f * sin(air);
	}
	if (keys['L' - 1])
	{
		acc.x -= 1.f * cos(air);
		acc.z += 1.f * sin(air);
	}
	if (keys['R' - 1])
	{
		acc.x += 1.f * cos(air);
		acc.z -= 1.f * sin(air);
	}

	if (acc.LengthSquared() > 0)
	{
		acc = acc.GetNormalized() * getPhysicsBody()->baseAcc;
		//std::cout << getPhysicsBody()->velocity.Length() << " <--"  << std::endl;

		//bool FW = (Dot(acc, getPhysicsBody()->direction) >= 0);
		//getPhysicsBody()->goingForward = FW;
		
		//if (Dot(getPhysicsBody()->direction, acc) >= 0)
		//{
		//std::cout << Dot(getPhysicsBody()->direction, acc.GetNormalized()) << std::endl;
		float vom = getPhysicsBody()->velocity.Length() / getPhysicsBody()->velocityLimit;
		if (Dot(getPhysicsBody()->direction, acc.GetNormalized()) >= -0.95f * vom)
		{
			if (ToDegrees(acos(Dot(acc.GetNormalized(), getPhysicsBody()->direction.GetNormalized()))) > maxSteer * vom * vom * dt)
			{
				//std::cout << acc.x << ", " << acc.z << std::endl;
				if (Dot(acc.GetNormalized(), getPhysicsBody()->direction) >= -0.99f)
				{				//std::cout << acc.x << ", " << acc.z << std::endl;

					float tVal = ToDegrees(acos(Dot(acc.GetNormalized(), getPhysicsBody()->direction)));

					//std::cout << tVal << std::endl;
					if (Dot(acc.GetNormalized(), getPhysicsBody()->direction) > 0.9999f)
					{
						acc += vec3(acc.z, 0, -acc.x) * 0.01f;
						acc = acc.GetNormalized();
						tVal = ToDegrees(acos(Dot(acc.GetNormalized(), getPhysicsBody()->direction)));
					}
					//std::cout << Dot(acc.GetNormalized(), getPhysicsBody()->direction) << std::endl;
					trueAcc = LP::SLERP(getPhysicsBody()->direction, acc.GetNormalized(), vec3(0, 0, 0), maxSteer * vom * vom * dt / tVal) * getPhysicsBody()->baseAcc;
					//std::cout << trueAcc.x << ", " << trueAcc.y << ", " << trueAcc.z << std::endl;
					//std::cout << "DIS PAT" << std::endl;
				}
			}
			else
			{
				trueAcc = acc;
				//std::cout << "NO DIS PAT" << std::endl;
			}
			//std::cout << "FORW!" << std::endl;
		}
		else// if (Dot(getPhysicsBody()->direction, acc.GetNormalized()) < -0.9f * getPhysicsBody()->velocity.Length() / getPhysicsBody()->velocityLimit)
		{
			//std::cout << "BACKW!" << std::endl;
			trueAcc = -getPhysicsBody()->direction * getPhysicsBody()->baseAcc;
			//if (Dot(trueAcc, getPhysicsBody()->velocity) > 0)
			//{
			//	trueAcc *= -1;
			//}
			//else
			//{
				if (trueAcc.Length() * dt > getPhysicsBody()->velocity.Length())
				{
					getPhysicsBody()->goingForward = !getPhysicsBody()->goingForward;
					vec3 dCheck = getPhysicsBody()->direction;
					if ((getPhysicsBody()->velocity + trueAcc * dt).LengthSquared() > 0)
					{
						dCheck = (getPhysicsBody()->velocity + trueAcc * dt).GetNormalized();
						if (!getPhysicsBody()->goingForward)
							dCheck *= -1;
					}
					if (Dot(dCheck, getPhysicsBody()->vDir) < 0)
						getPhysicsBody()->goingForward = !getPhysicsBody()->goingForward;
					//std::cout << "ARE YA SWAPPING?" << std::endl;
					//std::cout << Dot(getPhysicsBody()->velocity, getPhysicsBody()->direction) << std::endl;
					//std::cout << getPhysicsBody()->velocity.Length() << std::endl;
					//std::cout << trueAcc.Length() * dt << std::endl;
					//
					//std::cout << "-------" << std::endl;
				}
				//std::cout << "NO, IT DIS" << std::endl;
			//}
		}
		//}
		//else
		//{
		//	if (ToDegrees(acos(Dot(acc.GetNormalized(), -getPhysicsBody()->direction.GetNormalized()))) > maxSteer * getPhysicsBody()->velocity.Length() * dt)
		//	{
		//		trueAcc = LP::SLERP(-getPhysicsBody()->direction.GetNormalized(), acc.GetNormalized(), vec3(0, 0, 0), maxSteer * getPhysicsBody()->velocity.Length() * dt / ToDegrees(acos(Dot(acc.GetNormalized(), -getPhysicsBody()->direction.GetNormalized())))) * getPhysicsBody()->baseAcc;
		//		//std::cout << trueAcc.x << ", " << trueAcc.y << ", " << trueAcc.z << std::endl;
		//	}
		//	else
		//	{
		//		trueAcc = acc;
		//	}
		//}
		//if (Dot(getPhysicsBody()->velocity, getPhysicsBody()->direction) < 0)
		//{
		//	trueAcc = -getPhysicsBody()->velocity.GetNormalized() * getPhysicsBody()->baseAcc;
		//}
		//else
		//{
		//	if (ToDegrees(acos(Dot(acc.GetNormalized(), getPhysicsBody()->direction.GetNormalized()))) > maxSteer * getPhysicsBody()->velocity.Length() * dt)
		//	{
		//		trueAcc = LP::SLERP(getPhysicsBody()->direction.GetNormalized(), acc.GetNormalized(), vec3(0, 0, 0), maxSteer * getPhysicsBody()->velocity.Length() * dt / ToDegrees(acos(Dot(acc.GetNormalized(), getPhysicsBody()->direction.GetNormalized()))));
		//	}
		//	else
		//	{
		//		trueAcc = acc;
		//	}
		//}
		
		//if (Dot(getPhysicsBody()->velocity, acc) >= 0 && FW)
		//{
		//	if ()
		//}
	}


	//std::cout << trueAcc.Length() << std::endl;
	getPhysicsBody()->acceleration = trueAcc;
}

void Player::setCam(Camera * cam)
{
	pCam = cam;
}

vec3* Player::getTilt()
{
	return &vApart;
	//std::cout << vApart.x << ", " << vApart.y << ", " << vApart.z << std::endl;
}

Camera * Player::getCam()
{
	return pCam;
}
