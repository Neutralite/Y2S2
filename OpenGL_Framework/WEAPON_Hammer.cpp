#include "WEAPON_Hammer.h"

std::vector<wFrame> Hammer::FRAMES;

#define fileDir "../assets/Weapons/"

Hammer::Hammer()
{
	TT = TYPE_Hammer;
}

Hammer::~Hammer()
{
}

bool Hammer::weaponInit(std::string file)
{
	std::ifstream theWeapon;
	theWeapon.open(fileDir + file + ".txt");
	
	std::string parse;
	std::getline(theWeapon, parse);

	

	return false;
}

bool Hammer::tailoredCollision(GameObject * _GO)
{
	return false;
}

wFrame * Hammer::getFrameBefore(float t)
{
	for (unsigned int i = 0; i < FRAMES.size(); ++i)
	{
		if (FRAMES[i].timeOf > t)
		{
			if (i <= 1)
				return &FRAMES[0];
			else
				return &FRAMES[i - 2];
		}
	}

	return nullptr;
}

wFrame * Hammer::getFrameAt(float t)
{
	for (unsigned int i = 0; i < FRAMES.size(); ++i)
	{
		if (FRAMES[i].timeOf > t)
		{
			return &FRAMES[i - 1];
		}
	}

	return nullptr;
}

wFrame * Hammer::getFrameAfter(float t)
{
	for (unsigned int i = 0; i < FRAMES.size(); ++i)
	{
		if (FRAMES[i].timeOf > t)
		{
			return &FRAMES[i];
		}
	}

	return nullptr;
}

wFrame * Hammer::getFrame2After(float t)
{
	for (unsigned int i = 0; i < FRAMES.size(); ++i)
	{
		if (FRAMES[i].timeOf > t)
		{
			if (i >= FRAMES.size() - 1)
				return &FRAMES[FRAMES.size() - 1];
			else
				return &FRAMES[i + 1];
		}
	}

	return nullptr;
	return nullptr;
}

unsigned int Hammer::getDest()
{
	return destrType;
}

void Hammer::otherUpdates(float dt)
{
}
