#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	TT = TransformType::TYPE_GameObject;
}

GameObject::GameObject(Mesh * _mesh, Texture * _texture)
{
	setMesh(_mesh);
	setTexture(_texture);
	TT = TransformType::TYPE_GameObject;
}

GameObject::GameObject(Mesh * _mesh, std::vector<Texture*> &_textures)
{
	setMesh(_mesh);
	setTextures(_textures);
	TT = TransformType::TYPE_GameObject;
}

GameObject::~GameObject()
{
	
}

PhysicsBody * GameObject::getPhysicsBody()
{
	return &PB;
}

void GameObject::setPhysicsBody(PhysicsBody _PB)
{
	PB = _PB;
}

void GameObject::setMesh(Mesh * _mesh)
{
	mesh = _mesh;

	if (mesh)
	{
		doCull = true;
		tCenter = mesh->trueCenter;
		drawPoint1 = mesh->testPoint1;
		drawPoint2 = mesh->testPoint2;
	}
	else
	{
		doCull = false;
	}
}

Mesh * GameObject::getMesh()
{
	return mesh;
}

void GameObject::addTexture(Texture * _texture)
{
	textures.push_back(_texture);
}

void GameObject::setTexture(Texture * _texture)
{
	textures.clear();
	textures.push_back(_texture);
}

void GameObject::setTextures(std::vector<Texture*> &_textures)
{
	textures.clear();
	for (Texture* texture : _textures)
	{
		textures.push_back(texture);
	}
}

void GameObject::setShaderProgram(ShaderProgram * _shaderProgram)
{
	material = _shaderProgram;
}

ShaderProgram * GameObject::getShader()
{
	return material;
}

std::vector<Texture*>* GameObject::getTextures()
{
	return &textures;
}

void GameObject::draw()
{
	if (mesh)
	{
		material->bind();
		material->sendUniform("uModel", getLocalToWorld());
		int i = 0;
		for (Texture* texture : textures)
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		mesh->draw();
		for (Texture* texture : textures)
		{
			texture->unbind(--i);
		}
	}
}

void GameObject::dynamicDraw()
{
	if (mesh)
	{
		material->bind();
		material->sendUniform("uModel", getLocalToWorld());
		int i = 0;
		for (Texture* texture : textures)
		{
			texture->bind(i++);
		}
		mesh->dynamicDraw();
		for (Texture* texture : textures)
		{
			texture->unbind(--i);
		}
	}
}


void GameObject::doCollision(GameObject* _GO)
{
	if (_GO->getPhysicsBody()->getHB() && getPhysicsBody()->getHB())
	{
		if (_GO->getPhysicsBody()->getHB()->enabled && getPhysicsBody()->getHB()->enabled)
		{
			if (_GO->getPhysicsBody()->getHB()->dynamic)
			{

			}
			else if (_GO->getPhysicsBody()->getHB()->grass)
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					if (length(getVelocity()) > 0.f)
					{
						vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);
						vec3 BP1 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
						vec3 BP2 = -outward * length(getVelocity()) * getPhysicsBody()->getMass() * 0.15f;
						_GO->applySwing(BP1, BP2, 0.7f);
					}

					_GO->needsUpdate = true;
					needsUpdate = true;
				}
			}
			else
			{
				if (getPhysicsBody()->getHB()->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					vec3 centerToCollision = normalize(getPhysicsBody()->getHB()->closestPoint);
					vec3 outward = normalize(getPhysicsBody()->getHB()->outDir);

					centerToCollision.y = 0.f;
					outward.y = 0.f;

					if (dot(outward, getVelocity()) < 0)
					{
						pushAgainst += outward;
					}

					if (dot(outward, getVelocity()) < 0.f)
					{
						vec3 BP2 = getVelocity() * getPhysicsBody()->getMass() * 0.05f;
						vec3 BP1 = (-getVelocity() + outward * 2 * dot(outward, getVelocity())) * getPhysicsBody()->getMass() * 0.05f;
						if (length(BP1) > length(_GO->swingPoint1) || _GO->swingTime <= 0.f)
						{
							_GO->applySwing(BP1, BP2, 1.f);
							//std::cout << "HELLO" << std::endl;
						}

						_GO->needsUpdate = true;
						needsUpdate = true;
					}
				}
			}
		}
	}
}

vec3 GameObject::getVelocity()
{
	return PB.getVelocity();
}

vec3 GameObject::getAcceleration()
{
	return PB.getAcceleration();
}

vec3 GameObject::getAngularVelocity()
{
	return PB.getAngularVelocity();
}

vec3 GameObject::getAngularAcceleration()
{
	return PB.getAngularAcceleration();
}

void GameObject::applySwing(vec3 P1, vec3 P2, float ratio)
{
	swingPoint1 = P1;
	swingPoint2 = P2;
	swingTime = ratio * maxSwingTime;
}

void GameObject::initiateDestruction(int destrType, vec3 directionOutwards)
{
	TypeOfDestr = destrType;
	DirOfDestr = directionOutwards;
}

void GameObject::setInitials(vec3 iPos, vec3 iRot, vec3 iScale)
{
	initialPosition = iPos;
	initialRotation = iRot;
	initialScale = iScale;
}

void GameObject::resetToInitials()
{
	setLocalPos(initialPosition);
	setLocalRot(initialRotation);
	setScale(initialScale);

	needsUpdate = false;
	hasBeenUpdated = false;

	destroying = false;
	destroyed = false;

	getPhysicsBody()->reset();
}

