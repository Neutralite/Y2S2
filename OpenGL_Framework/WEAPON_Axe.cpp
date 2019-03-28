#include "WEAPON_Axe.h"
#include "ResourceManager.h"

std::vector<wFrame> Axe::FRAMES;
float Axe::COOL_DOWN;

Axe::Axe()
{
	TT = TransformType::TYPE_Axe;
}

Axe::~Axe()
{
}

bool Axe::weaponInit()
{
	wFrame W;

	Weapon* _W = new Axe;

	W.position = vec3(0, 0, 0);
	W.rotationAngles = vec3(0, 0, 0);
	W.duration = 0.0f;
	W.scale = vec3(1, 1, 1);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Axe");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("AxeBaseColours");
	W.LT = wFrame::LerpType::LT_LINEAR;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	for (int i = 0; i < 50; i++)
	{
		W.position = vec3(0, 0, -0.35f * (float)i);
		W.rotationAngles = vec3(0, -10.f * (float)i, 0);
		W.duration = 0.005f;
		W.scale = vec3(1, 1, 1);
		W.MT = wFrame::MutationType::LINEAR;
		W.weapMesh = ResourceManager::getMesh("Axe");
		W.Attached = _W;
		W.BLOWUP = true;
		W.SP = ResourceManager::getShader("COMIC_SETUP");
		W.weapTex = rm::getMaterial("AxeBaseColours");
		W.LT = wFrame::LerpType::LT_LINEAR;
		W.hitboxOffset = vec3(0, 0, 0);
		FRAMES.push_back(W);
	}

	W.position = vec3(0, 0, -16.8f);
	W.rotationAngles = vec3(0, -800.f, 0);
	W.duration = 0.01f;
	W.scale = vec3(0, 0, 0);
	W.MT = wFrame::MutationType::LINEAR;
	W.weapMesh = ResourceManager::getMesh("Axe");
	W.Attached = _W;
	W.BLOWUP = false;
	W.SP = ResourceManager::getShader("COMIC_SETUP");
	W.weapTex = rm::getMaterial("AxeBaseColours");
	W.LT = wFrame::LerpType::LT_LINEAR;
	W.hitboxOffset = vec3(0, 0, 0);
	FRAMES.push_back(W);

	float accum = 0;
	for (unsigned int i = 0; i < FRAMES.size(); i++)
	{
		FRAMES[i].timeOf = accum;
		accum += FRAMES[i].duration;
	}

	_W->setName("AXE");
	_W->Impact = ResourceManager::getHitbox("AXE_HITBOX");
	ResourceManager::addEntity(_W);

	COOL_DOWN = 0.3f;

	return true;
}

std::vector<wFrame>* Axe::getFL()
{
	return &FRAMES;
}

unsigned int Axe::getDest()
{
	return destrType;
}

void Axe::otherUpdates(float dt)
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

float Axe::frameWarp(float t)
{
	return t;
}

float Axe::coolDownTime()
{
	return COOL_DOWN;
}
