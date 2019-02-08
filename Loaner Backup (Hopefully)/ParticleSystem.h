#pragma once
#include "MESH.h"
#include <vector>
#include "Transform.h"

class ParticleSystem;

class Particle
{
public:
	Particle();
	~Particle();

	void applyForce(vec3 F);
	void applyTorque(vec3 T);
	void applyVelocity(vec3 V);
	void applyAngularVel(vec3 V);

	void applyScale(float  S);
	void applyScaleOverTime(float S);
	bool isFinished();
	
	void setDT(float _dt);
	void setTotalLife(float TL);
	void setCurrentLife(float CL);
	float getCurrentLife();
	float getTotalLife();

	void setStartPosition(vec3 SP);

	ParticleSystem* getMiniSys();
	void setMiniSys(ParticleSystem* _PS);

	Transform t;
	vec3 velocity;
	vec3 acceleration;
	vec3 force;
	vec3 AngVelocity;
	vec3 AngAcceleration;
	vec3 torque;

	vec3 startPosition;
	vec3 startRotation;

	void updateParticle();
	void resetParticle();

	float mass;

	float size;
	float sizeOverTime;
	float initialSize;

	float totalLife;
	float currentLife;

	float dt;

	ParticleSystem* miniSystem = nullptr;
};

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(Mesh* M, unsigned int amnt);
	~ParticleSystem();

	unsigned int getAmnt();
	void setMesh(Mesh* M);
	Mesh* getMesh();

	void setDT(float _dt);

	virtual void updateParticles(float _dt);
	void setPlaying(bool b);

	void restart();

	Mesh* mesh;
	std::vector<Particle*> p;
	float dt;
	bool playing;
};