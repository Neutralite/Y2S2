#pragma once
#include <GL\glew.h>
#include "MESH.h"
#include "Transform.h"
#include "TextureLayer.h"
#include "ParticleSystem.h"
#include "HITBOX.h"
#include "LIGHT_SOURCE.h"
#include "PhysicsBody.h"
#include "Camera.h"

class Section;
class Object
{
public:
	Object();
	Object(std::string &ObjFileName, std::vector<Mesh*>* _MESH_LIST, std::vector<TextureLayer*> _TEXTURE_LIST);
	~Object();

	void AttachObject(Object* obj);
	void RemoveObject(Object* obj);
	void RemoveObject(int index);
	void RemoveObject(std::string Identity);
	void AttachMesh(Mesh* _MESH);
	void setPosition(vec3 pos);
	void setRotation(vec3 rot);
	void TranslateBy(vec3 pos);
	void RotateBy(vec3 rot);
	void updateTransforms(float dt);
	void hide(bool b) { isVisible = !b; };
	void invertVisible() { isVisible = !isVisible; }

	bool visible() { return isVisible; }
	Mesh* getMesh() { return mesh; }

	Object* getChild(unsigned int i);
	Transform* getOrientation();

	std::string getIdentity();
	void setIdentity(std::string _NAME);
	unsigned int getNumberOfChildren();

	void addTrigger(std::string T);
	void removeTriggers(std::string T);
	void checkTriggers(std::string T);

	void addTexture(TextureLayer* TL);
	void removeTexture(TextureLayer* TL);
	void clearTextures();
	void reorderTextures();
	std::vector<TextureLayer*> getTextures();
	TextureLayer* getTexture(int index);

	void addParticleSystem(ParticleSystem* _PS);
	void updateParticleSystems(float dt);
	void removeParticleSystem(ParticleSystem* _PS);
	void resetParticleSystem(ParticleSystem* _PS);
	void resetAllParticleSystems();
	void setPlayingParticleSystem(ParticleSystem* _PS, bool _PLAYING);
	void setAllPlayingParticleSystem(bool _PLAYING);

	void setPhysicsBody(PhysicsBody PB);
	PhysicsBody* getPhysicsBody();

	void setBoundingRadius(float BR);
	float getBoundingRadius();
	
	void setIsBasePlate(bool _ISBASE);
	bool isBasePlate();

	void setLight(Light* _LIGHT);
	Light* getLight();

	void addTarget(Mesh* target);
	std::vector<Mesh*>* getTargets();
	Mesh* getTarget(int index);
	void removeTarget(Mesh* target);
	void removeTarget(int index);
	void removeTarget(std::string Name);

	virtual void setCam(Camera* cam);
	virtual Camera* getCam();
	virtual void updatePhysics(float dt);
	virtual void giveInput(char A, bool B);

	void setBezPointOut(vec3 p1, vec3 p2);
	void resetTimeOnBez();
	void setBezTimeToHalf();
	void setBezTimeLimit(float _LIM);
	void updateBezShake(float dt);
	float getTimeOnBez();
	vec3 getB1();
	vec3 getB2();
	virtual vec3* getTilt();

	bool isDestroyed();
	bool isBeingDestroyed();
	void setDestroyed(bool isDest);
	void setBeingDestroyed(bool isDest);
	void setHP(float HP);
	float getHP();
	void setMaxHP(float MH);
	float getMaxHP();

	void setBasePosition(vec3 basePos);
	void setBaseRotation(vec3 baseRot);
	vec3 getBasePosition();
	vec3 getBaseRotation();

	float YEET = 0.f;

private:
	bool isVisible = true;
	bool isBase = false;

	float boundingRadius = 0.f;

	std::string identity;
	std::vector<std::string>triggers;
	std::vector<Object*> children;
	std::vector<Section*> onTopOf;
	std::vector<Mesh*> TARGETS;
	std::vector<TextureLayer*> textures;
	Light* light = nullptr;
	
	vec3 bezP1 = vec3(0, 0, 0);
	vec3 bezP2 = vec3(0, 0, 0);
	vec3 basePosition = vec3(0, 0, 0);
	vec3 baseRotation = vec3(0, 0, 0);
	vec3* shouldNeverBeCalled;
	float tRem = 0.f;
	float tTop = 0.2f;

	float health = 0.f;
	float maxHealth = 0.f;

	bool destroyed = false;
	bool beingDestroyed = false;

	Object* parent;
	Mesh* mesh;
	PhysicsBody BODY;
	Transform orientation;
	std::vector<ParticleSystem*> effects;
};