#include "Object.h"
#include "Section.h"
#include <fstream>

Object::Object()
{
}

Object::~Object()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

Object::Object(std::string &ObjFileName, std::vector<Mesh*>* _MESH_LIST, std::vector<TextureLayer*> _TEXTURE_LIST)
{
	std::ifstream input(ObjFileName);

	std::vector<std::string> data;
	std::string temp;

	while (getline(input, temp))
	{
		data.push_back(temp);
	}


}

void Object::AttachObject(Object * obj)
{
	obj->parent = this;
	children.push_back(obj);
}

void Object::RemoveObject(Object * obj)
{
	for (int i = children.size() - 1; i >= 0; i--)
		if (children[i] == obj)
		{
			children[i]->parent = nullptr;
			children.erase(children.begin() + i);
		}
}

void Object::RemoveObject(int index)
{
	children[index]->parent = nullptr;
	children.erase(children.begin() + index);
}

void Object::RemoveObject(std::string Identity)
{
	for (int i = children.size() - 1; i >= 0; i--)
		if (children[i]->getIdentity() == Identity)
		{
			children[i]->parent = nullptr;
			children.erase(children.begin() + i);
		}
}

void Object::AttachMesh(Mesh * _MESH)
{
	mesh = _MESH;
}

void Object::setPosition(vec3 pos)
{
	orientation.setPosition(pos);
}

void Object::setRotation(vec3 rot)
{
	orientation.setRotationAngle(rot);
}

void Object::TranslateBy(vec3 pos) {
	orientation.setPosition(orientation.getPosition() + pos);
}
void Object::RotateBy(vec3 rot) {
	orientation.setRotationAngle(orientation.getRotationAngle() + rot);
}

void Object::updateTransforms(float dt)
{
	updatePhysics(dt);
	orientation.update(dt);
	for (int i = 0; i < getNumberOfChildren(); i++)
	{
		children[i]->updateTransforms(dt);
	}
}

Object* Object::getChild(unsigned int i)
{
	return children[i];
}

Transform* Object::getOrientation()
{
	return &orientation;
}

std::string Object::getIdentity()
{
	return identity;
}

void Object::setIdentity(std::string _NAME)
{
	identity = _NAME;
}

unsigned int Object::getNumberOfChildren()
{
	return children.size();
}

void Object::addTexture(TextureLayer * TL)
{
	int i = 0;
	bool added = false;
	while (i < textures.size() && !added)
	{
		if (textures[i]->getDepth() < TL->getDepth())
		{
			textures.insert(textures.begin() + i, TL);
			added = true;
		}
		i++;
	}
	if (!added)
	{
		textures.push_back(TL);
	}
}

void Object::removeTexture(TextureLayer * TL)
{
	for (int i = textures.size() - 1; i >= 0; i--)
		if (textures[i] == TL)
			textures.erase(textures.begin() + i);
}

void Object::clearTextures()
{
	textures.clear();
}

void Object::reorderTextures()
{
	for (int i = 0; i < textures.size() - 1; i++)
	{
		for (int j = textures.size() - 1; j > i; j++)
		{
			if (textures[i]->getDepth() > textures[j]->getDepth())
			{
				TextureLayer* temp = textures[i];
				textures[i] = textures[j];
				textures[j] = temp;
			}
		}
	}
}

std::vector<TextureLayer*> Object::getTextures()
{
	return textures;
}

TextureLayer * Object::getTexture(int index)
{
	if (index >= textures.size())
		return nullptr;
	return textures[index];
}

void Object::addParticleSystem(ParticleSystem * _PS)
{
	effects.push_back(_PS);
}

void Object::updateParticleSystems(float dt)
{
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->updateParticles(dt);
	}
}

void Object::removeParticleSystem(ParticleSystem * _PS)
{
	for (int i = effects.size() - 1; i >= 0; i--)
	{
		if (effects[i] == _PS)
		{
			effects.erase(effects.begin() + i);
		}
	}
}

void Object::resetParticleSystem(ParticleSystem * _PS)
{
	for (int i = effects.size() - 1; i >= 0; i--)
	{
		if (effects[i] == _PS)
		{
			effects[i]->restart();
		}
	}
}

void Object::resetAllParticleSystems()
{
	for (int i = effects.size() - 1; i >= 0; i--)
	{
		effects[i]->restart();
	}
}

void Object::setPlayingParticleSystem(ParticleSystem* _PS, bool _PLAYING)
{
	for (int i = effects.size() - 1; i >= 0; i--)
	{
		if (effects[i] == _PS)
		{
			effects[i]->setPlaying(_PLAYING);
		}
	}
}

void Object::setAllPlayingParticleSystem(bool _PLAYING)
{
	for (int i = effects.size() - 1; i >= 0; i--)
	{
		effects[i]->setPlaying(_PLAYING);
	}
}

void Object::setPhysicsBody(PhysicsBody PB)
{
	BODY = PB;
}

PhysicsBody * Object::getPhysicsBody()
{
	return &BODY;
}

void Object::setBoundingRadius(float BR)
{
	boundingRadius = BR;
}

float Object::getBoundingRadius()
{
	return boundingRadius;
}

void Object::setIsBasePlate(bool _BASE)
{
	isBase = _BASE;
}

bool Object::isBasePlate()
{
	return isBase;
}

void Object::setLight(Light * _LIGHT)
{
	light = _LIGHT;
}

Light * Object::getLight()
{
	return light;
}

void Object::addTarget(Mesh * target)
{
	TARGETS.push_back(target);
}

std::vector<Mesh*>* Object::getTargets()
{
	return &TARGETS;
}

Mesh * Object::getTarget(int index)
{
	return TARGETS[index];
}

void Object::removeTarget(Mesh * target)
{
	for (int i = TARGETS.size() - 1; i >= 0; i--)
		if (TARGETS[i] == target)
			TARGETS.erase(TARGETS.begin() + i);

}

void Object::removeTarget(int index)
{
	TARGETS.erase(TARGETS.begin() + index);
}

void Object::removeTarget(std::string Name)
{
	for (int i = TARGETS.size() - 1; i >= 0; i--)
		if (TARGETS[i]->ID == Name)
			TARGETS.erase(TARGETS.begin() + i);
}

void Object::setCam(Camera * cam)
{
}

Camera * Object::getCam()
{
	return nullptr;
}

void Object::updatePhysics(float dt)
{
}

void Object::giveInput(char A, bool B)
{
}

void Object::setBezPointOut(vec3 p1, vec3 p2)
{
	bezP1 = p1;
	bezP2 = p2;
}

void Object::resetTimeOnBez()
{
	tRem = tTop;
}

void Object::setBezTimeToHalf()
{
	tRem = tTop * 0.5f;
}

void Object::setBezTimeLimit(float _LIM)
{
	tTop = _LIM;
}

void Object::updateBezShake(float dt)
{
	tRem -= dt;
}

float Object::getTimeOnBez()
{
	return tRem / tTop;
}

vec3 Object::getB1()
{
	return bezP1;
}

vec3 Object::getB2()
{
	return bezP2;
}

vec3* Object::getTilt()
{
	return nullptr;
}

bool Object::isDestroyed()
{
	return destroyed;
}

void Object::setDestroyed(bool isDest)
{
	destroyed = isDest;
}

void Object::setHP(float HP)
{
	health = HP;
}

float Object::getHP()
{
	return health;
}

void Object::setMaxHP(float MH)
{
	maxHealth = MH;
}

float Object::getMaxHP()
{
	return maxHealth;
}

void Object::setBasePosition(vec3 basePos)
{
	basePosition = basePos;
}

void Object::setBaseRotation(vec3 baseRot)
{
	baseRotation = baseRot;
}

vec3 Object::getBasePosition()
{
	return basePosition;
}

vec3 Object::getBaseRotation()
{
	return baseRotation;
}
