#pragma once
#include "GameObject.h"
#include "Weapon.h"

class Player : public GameObject
{
public:
	enum PLAYER_IN
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		ATTACK
	};

	enum PLAYER_TYPE
	{
		TRUCK,
		TANK,
		WRECKING_BALL,
		BULLDOZER
	};

	Player();
	~Player();
	
	void playerInit(PLAYER_TYPE PT);

	void setBob(float bob);
	void sendInput(bool type, PLAYER_IN pin);
	float getBob();
	void update(float dt);
	void performAttack(float dt);
	void updatePhysics(float dt);
	void draw();

	void attachWeapon(Weapon* W);
	Weapon* getWeapon();

	vec3 getVelocity();
	vec3 getAcceleration();
	vec3 getAngularVelocity();
	vec3 getAngularPosition();

	void doCollision(GameObject* _GO);

	//void initiateDestruction(int destrType, vec3 directionOutwards);
	void DestructionSequence(float dt);

	void resetToInitials();

	void terminateEngine();

	float Yangle = 0.f;
	bool sendATTACK = false;
	float steeringMultiplier = 1.f;

	int POINT_TOTAL = 0;
	int LERP_TOTAL = 0;

	float playerHP = 1.f;
	float maxHP = 1.f;
	float LERP_HP = 1.f;
	float respawnTimer = 0.f;

	int AttacksLeft = 0;

	int playerNumber = 0;

	int respawnPoint = 0;
private:
	Weapon* attack = nullptr;
	float attackTimer = 0.f;

	bool mUP = false;
	bool mDOWN = false;
	bool mLEFT = false;
	bool mRIGHT = false;
	bool mATTACK = false;

	PhysicsBody *Engine;
	vec3 direction;
	vec3 right;
	vec3 catchUpVel;
	vec3 lerpVel;

	float randomBob;
	float baseAcceleration;
	float steeringForce;
};