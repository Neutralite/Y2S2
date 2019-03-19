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

	W.position = vec3(0, 0.03f, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 1.5f;
	W.scale = vec3(1, 1, 1);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("MINE");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("MINE_TEXTURE");
	W.LT = wFrame::LerpType::LT_LINEAR;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(0, 0.03f, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 0.001f;
	W.scale = vec3(1, 1, 1);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("TESTING_BASE_PLATE");
	W.Attached = _W;
	W.BLOWUP = true;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = ResourceManager::getMaterial("BLAST_RADIUS");
	W.LT = wFrame::LerpType::LT_LINEAR;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(0, 0.04f, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 0.6f;
	W.scale = vec3(2.0f, 2.0f, 2.0f);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("TESTING_BASE_PLATE");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = ResourceManager::getMaterial("BLAST_RADIUS");
	W.LT = wFrame::LerpType::LT_HIGH_POWER;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	//for (unsigned int i = 0; i < FRAMES.size(); i++)
	//{
	//	std::cout << FRAMES[i].LT << std::endl;
	//}

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

std::vector<wFrame>* Mine::getFL()
{
	return &FRAMES;
}

unsigned int Mine::getDest()
{
	return destrType;
}

void Mine::otherUpdates(float dt)
{
	while (currentFrame < FRAMES.size() && timeActive > FRAMES[currentFrame].timeOf + FRAMES[currentFrame].duration)
	{
		//std::cout << FRAMES[currentFrame].LT << ", " << currentFrame << ", " << timeActive << std::endl;
		currentFrame++;
		if (currentFrame < FRAMES.size())
			if (FRAMES[currentFrame].BLOWUP)
			{
				hitboxOffset = FRAMES[currentFrame].hitboxOffset;
				//std::cout << hitboxOffset << std::endl;
				flingHitbox++;
			}
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
