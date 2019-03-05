#pragma once
#include "GameObject.h"
#include <fstream>

class Weapon;

class wFrame
{
public:
	vec3 position;
	vec3 rotationAngles;
	vec3 scale;
	float duration; //How long it happens for
	float timeOf; //When it happens
	Mesh* weapMesh;
	std::vector<Texture*> weapTex;
	ShaderProgram *SP;
	Hitbox2D* HB = nullptr;

	Weapon* Attached = nullptr;

	enum MutationType
	{
		CATMULL,
		LINEAR
	};

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

	std::vector<float> impactTimes;
	
	virtual bool tailoredCollision(GameObject* _GO);
	virtual wFrame* getFrameBefore(float t) = 0;
	virtual wFrame* getFrameAt(float t) = 0;
	virtual wFrame* getFrameAfter(float t) = 0;
	virtual wFrame* getFrame2After(float t) = 0;

	bool timeToDie = false;

	virtual unsigned int getDest() = 0;

	void update(float dt);
	void draw();

	virtual void otherUpdates(float dt) = 0;

	virtual float frameWarp(float t) = 0;

	unsigned int currentFrame = 0;

	virtual float coolDownTime() = 0;

	mat4 getLocalToWorld() const;
private:

};