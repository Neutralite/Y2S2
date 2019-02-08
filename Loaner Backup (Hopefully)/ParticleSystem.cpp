#include "ParticleSystem.h"

Particle::Particle()
{
}

Particle::~Particle()
{
	if (miniSystem != nullptr)
	{
		delete miniSystem;
		miniSystem = nullptr;
	}
}

void Particle::applyForce(vec3 F)
{
	acceleration += F / mass;
}

void Particle::applyTorque(vec3 T)
{
	AngAcceleration += T / mass;
}

void Particle::applyVelocity(vec3 V)
{
	velocity += V;
}

void Particle::applyAngularVel(vec3 V)
{
	AngVelocity += V;
}

void Particle::applyScale(float S)
{
	size += S;
}

void Particle::applyScaleOverTime(float S)
{
	sizeOverTime = S;
}

bool Particle::isFinished()
{
	return currentLife <= 0;
}

void Particle::setDT(float _dt)
{
	dt = _dt;
}

void Particle::setTotalLife(float TL)
{
	totalLife = TL;
}

void Particle::setCurrentLife(float CL)
{
	currentLife = CL;
}

float Particle::getCurrentLife()
{
	return currentLife;
}

float Particle::getTotalLife()
{
	return totalLife;
}

void Particle::setStartPosition(vec3 SP)
{
	startPosition = SP;
}

ParticleSystem * Particle::getMiniSys()
{
	return miniSystem;
}

void Particle::setMiniSys(ParticleSystem * _PS)
{
	miniSystem = _PS;
}

void Particle::updateParticle()
{
	if (miniSystem != nullptr)
		miniSystem->updateParticles(dt);
	currentLife -= dt;

	if (isFinished())
	{
		resetParticle();
	}

	velocity += acceleration * dt;
	t.setPosition(t.getPosition() + velocity * dt);
	AngVelocity += AngAcceleration * dt;
	t.setRotationAngle(t.getRotationAngle() + AngVelocity * dt);
	size += sizeOverTime * dt;
	t.setScale(t.getScale() + size);
}

void Particle::resetParticle()
{
	currentLife = 0;
	t.setPosition(startPosition);
	t.setRotationAngle(startRotation);
	velocity = vec3(0, 0, 0);
	acceleration = vec3(0, 0, 0);
	AngVelocity = vec3(0, 0, 0);
	AngAcceleration = vec3(0, 0, 0);
	t.setScale(initialSize);
}

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(Mesh * M, unsigned int amnt)
{
	mesh = M;
	for (int i = 0; i < amnt; i++)
	{
		Particle* part = new Particle;
		p.push_back(part);
	}
}

ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < p.size(); i++)
	{
		delete p[i];
		p[i] = nullptr;
	}
}

unsigned int ParticleSystem::getAmnt()
{
	return p.size();
}

void ParticleSystem::setMesh(Mesh * M)
{
	mesh = M;
}

Mesh * ParticleSystem::getMesh()
{
	return mesh;
}

void ParticleSystem::setDT(float _dt)
{
	dt = _dt;
}

void ParticleSystem::updateParticles(float _dt)
{
	for (int i = 0; i < p.size(); i++)
	{
		p[i]->updateParticle();
	}
}

void ParticleSystem::setPlaying(bool b)
{
	playing = b;
}

void ParticleSystem::restart()
{
	for (int i = 0; i < p.size(); i++)
		p[i]->resetParticle();
}
