#pragma once
#include "GameObject.h"
#include <fstream>
#include <iostream>

class Weapon;

class wFrame
{
public:
	vec3 position;
	vec3 rotationAngles;
	vec3 scale;
	vec3 hitboxOffset;
	float duration; //How long it happens for
	float timeOf; //When it happens
	Mesh* weapMesh;
	Material* weapTex;
	ShaderProgram *SP;

	Weapon* Attached = nullptr;

	enum MutationType
	{
		CATMULL,
		LINEAR
	};

	enum LerpType
	{
		LT_LINEAR,
		LT_SQUARE,
		LT_ONE_MINUS_SQUARE,
		LT_SINUSOIDAL,
		LT_HIGH_POWER
	};

	LerpType LT;
	MutationType MT;

	bool BLOWUP = false;

	float tFunc(float t);
};

class Weapon : public GameObject
{
public:
	Weapon();
	~Weapon();

	float timeActive = 0.f; //Running Time of Weapon
	
	Hitbox2D* Impact;
	mat4 worldLocation;
	vec3 hitboxOffset;

	std::vector<float> impactTimes;
	
	bool tailoredCollision(GameObject* _GO);
	virtual std::vector<wFrame>* getFL() = 0;
	virtual wFrame* getFrameBefore(float t);
	virtual wFrame* getFrameAt(float t);
	virtual wFrame* getFrameAfter(float t);
	virtual wFrame* getFrame2After(float t);

	bool timeToDie = false;

	int flingHitbox = 0;

	virtual unsigned int getDest() = 0;

	void update(float dt);
	void draw();

	virtual void otherUpdates(float dt) = 0;

	virtual float frameWarp(float t) = 0;

	unsigned int currentFrame = 0;

	virtual float coolDownTime() = 0;

	mat4 getLocalToWorld() const;

	int ownedPlayer = 0;
private:

};