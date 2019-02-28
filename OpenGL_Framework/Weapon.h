#pragma once
#include "GameObject.h"
#include <fstream>


class wFrame
{
public:
	vec3 position;
	vec3 rotationAngles;
	vec3 scale;
	float duration; //How long it happens for
	float timeOf; //When it happens

	enum MutationType
	{
		CATMULL,
		LINEAR
	};

	MutationType MT;

	float tFunc(float t);
};

class Weapon : public GameObject
{
public:
	Weapon();
	~Weapon();

	virtual bool weaponInit(std::string file) = 0;
	float timeActive = 0.f; //Running Time of Weapon
	
	Hitbox2D* Impact;
	std::vector<float> impactTimes;
	
	virtual bool tailoredCollision(GameObject* _GO) = 0;
	virtual wFrame* getFrameBefore(float t) = 0;
	virtual wFrame* getFrameAt(float t) = 0;
	virtual wFrame* getFrameAfter(float t) = 0;
	virtual wFrame* getFrame2After(float t) = 0;

	bool timeToDie = false;

	virtual unsigned int getDest() = 0;
	void update(float dt);
	virtual void otherUpdates(float dt) = 0;
private:

};