#include "WEAPON_Mine.h"
#include "ResourceManager.h"

std::vector<wFrame> Mine::FRAMES;
float Mine::COOL_DOWN;

Mine::Mine()
{
	TT = Transform::TransformType::TYPE_Mine;
}

Mine::~Mine()
{
}

bool Mine::weaponInit()
{
	wFrame W;

	Weapon* _W = new Mine;

	W.position = vec3(0, 0, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 2.f;
	W.scale = vec3(1, 1, 1);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("MINE");
	W.Attached = _W;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex.push_back(ResourceManager::getTexture("MINE_TEXTURE"));
	FRAMES.push_back(W);
	W.weapTex.clear();

	W.position = vec3(0, 0, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 1.f;
	W.scale = vec3(1, 1, 1);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("EXPLOSION");
	W.Attached = _W;
	W.BLOWUP = true;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex.push_back(ResourceManager::getTexture("EXPLOSION"));
	FRAMES.push_back(W);
	W.weapTex.clear();

	float accum = 0;
	for (unsigned int i = 0; i < FRAMES.size(); i++)
	{
		FRAMES[i].timeOf = accum;
		accum += FRAMES[i].duration;
	}

	_W->setName("MINE");
	_W->Impact = ResourceManager::getHitbox("EXPLOSION_HITBOX");
	ResourceManager::addEntity(_W);

	COOL_DOWN = 0.1f;

	return true;
}

//bool Mine::tailoredCollision(GameObject * _GO)
//{
//	if (_GO->getPhysicsBody()->getHB() && getPhysicsBody()->getHB())
//	{
//		if (_GO->getPhysicsBody()->getHB()->enabled && getPhysicsBody()->getHB()->enabled)
//		{
//			if (_GO->getPhysicsBody()->getHB()->dynamic)
//			{
//
//			}
//			else if (_GO->getPhysicsBody()->getHB()->grass)
//			{
//				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
//				{
//					if (length(getVelocity()) > 0.f)
//					{
//						vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);
//						vec3 BP1 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
//						vec3 BP2 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
//						_GO->applySwing(BP1, BP2, 0.7f);
//					}
//
//					_GO->needsUpdate = true;
//					needsUpdate = true;
//				}
//			}
//			else
//			{
//				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
//				{
//					vec3 centerToCollision = normalize(getPhysicsBody()->getHB()->closestPoint);
//					vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);
//
//					centerToCollision.y = 0.f;
//					outward.y = 0.f;
//
//					if (dot(outward, getVelocity()) < 0)
//					{
//						pushAgainst += outward;
//					}
//
//					if (dot(outward, getVelocity()) < 0.f)
//					{
//						vec3 BP2 = getVelocity() * getPhysicsBody()->getMass() * 0.05f;
//						vec3 BP1 = (-getVelocity() + outward * 2 * dot(outward, getVelocity())) * getPhysicsBody()->getMass() * 0.05f;
//						if (length(BP1) > length(_GO->swingPoint1) || _GO->swingTime <= 0.f)
//						{
//							_GO->applySwing(BP1, BP2, 1.f);
//							//std::cout << "HELLO" << std::endl;
//						}
//
//						_GO->needsUpdate = true;
//						needsUpdate = true;
//					}
//				}
//			}
//		}
//	}
//	return false; //Do this next Jakob. Do it. Or else. --Sincerely, Jakob
//}

wFrame * Mine::getFrameBefore(float t)
{
	if (currentFrame >= 2)
		return &FRAMES[currentFrame - 2];
	else
		return nullptr;
}

wFrame * Mine::getFrameAt(float t)
{
	if (currentFrame >= 1)
		return &FRAMES[currentFrame - 1];
	else
		return nullptr;
}

wFrame * Mine::getFrameAfter(float t)
{
	return &FRAMES[currentFrame];
}

wFrame * Mine::getFrame2After(float t)
{
	if (currentFrame < FRAMES.size() - 1)
		return &FRAMES[currentFrame + 1];
	else
		return nullptr;
}

unsigned int Mine::getDest()
{
	return destrType;
}

void Mine::otherUpdates(float dt)
{
	while (currentFrame < FRAMES.size() && timeActive > FRAMES[currentFrame].timeOf + FRAMES[currentFrame].duration)
	{
		currentFrame++;
		if (currentFrame < FRAMES.size())
			if (FRAMES[currentFrame].BLOWUP)
				flingHitbox++;
	}

	if (currentFrame >= FRAMES.size())
	{
		timeToDie = true;
	}
}

float Mine::frameWarp(float t)
{
	return t;
}

float Mine::coolDownTime()
{
	return COOL_DOWN;
}
