#include "WEAPON_Hammer.h"
#include "ResourceManager.h"

std::vector<wFrame> Hammer::FRAMES;
float Hammer::COOL_DOWN;

Hammer::Hammer()
{
	TT = TYPE_Hammer;
}

Hammer::~Hammer()
{
}

bool Hammer::weaponInit()
{
	wFrame W;

	Weapon* _W = new Hammer;

	W.position = vec3(0, 0, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 0.f;
	W.scale = vec3(0, 0, 0);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Hammer");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("HammerBaseColours");
	W.LT = wFrame::LerpType::LT_ONE_MINUS_SQUARE;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(1.f, 4.f, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 0.2f;
	W.scale = vec3(1, 5, 5);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Hammer");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("HammerBaseColours");
	W.LT = wFrame::LerpType::LT_ONE_MINUS_SQUARE;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(1.2f, 0.4f, 0);
	W.rotationAngles = vec3(0, 0, -90);
	W.duration = 0.07f;
	W.scale = vec3(5, 5, 5);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Hammer");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("HammerBaseColours");
	W.LT = wFrame::LerpType::LT_SQUARE;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(1.2f, 1.3f, 0);
	W.rotationAngles = vec3(0, 0, -90);
	W.duration = 0.15f;
	W.scale = vec3(3, 5, 5);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Hammer");
	W.Attached = _W;
	W.BLOWUP = true;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("HammerBaseColours");
	W.LT = wFrame::LerpType::LT_HIGH_POWER;
	W.hitboxOffset = vec3(7.5f, 0, 0);
	FRAMES.push_back(W);

	W.position = vec3(0.f, 0.f, 0);
	W.rotationAngles = vec3(0, 0, -90);
	W.duration = 0.06f;
	W.scale = vec3(0, 0, 0);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Hammer");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("HammerBaseColours");
	W.LT = wFrame::LerpType::LT_ONE_MINUS_SQUARE;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	float accum = 0;
	for (unsigned int i = 0; i < FRAMES.size(); i++)
	{
		FRAMES[i].timeOf = accum;
		accum += FRAMES[i].duration;
	}

	_W->setName("HAMMER");
	_W->Impact = ResourceManager::getHitbox("HAMMER_HITBOX");
	ResourceManager::addEntity(_W);

	COOL_DOWN = 0.15f;

	return true;
}

std::vector<wFrame>* Hammer::getFL()
{
	return &FRAMES;
}

unsigned int Hammer::getDest()
{
	return destrType;
}

void Hammer::otherUpdates(float dt)
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

float Hammer::frameWarp(float t)
{
	return t;
}

float Hammer::coolDownTime()
{
	return COOL_DOWN;
}
