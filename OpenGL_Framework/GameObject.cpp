#include "GameObject.h"
#include <iostream>

#define PI 3.14159265358979323846f

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

bool GameObject::initGameObjects()
{
	return true;
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
	//textures.push_back(_texture);
	material->textures.push_back(_texture);
}

void GameObject::setTexture(Texture * _texture)
{
	material->textures.clear();
	material->textures.push_back(_texture);
}

void GameObject::setTextures(std::vector<Texture*> &_textures)
{
	material->textures.clear();
	for (Texture* texture : _textures)
	{
		material->textures.push_back(texture);
	}
}

void GameObject::setShaderProgram(ShaderProgram * _shaderProgram)
{
	if (material)
		material->shader = _shaderProgram;
}

void GameObject::setMaterial(Material * _MAT)
{
	material = _MAT;
}

Material * GameObject::getMaterial()
{
	return material;
}

ShaderProgram * GameObject::getShader()
{
	return material->shader;
}

std::vector<Texture*>* GameObject::getTextures()
{
	return &material->textures;
}

void GameObject::draw()
{
	if (mesh && material && !HIDE)
	{
		material->bind();
		material->sendUniforms();
		material->shader->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		int i = 0;
		for (Texture* texture : material->textures)
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		mesh->draw();
		for (Texture* texture : material->textures)
		{
			texture->unbind(--i);
		}
	}
}

void GameObject::dynamicDraw()
{
	if (mesh && !HIDE)
	{
		material->bind();
		material->sendUniforms();
		material->shader->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		int i = 0;
		for (Texture* texture : material->textures)
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		mesh->draw();
		for (Texture* texture : material->textures)
		{
			texture->unbind(--i);
		}
	}
}


void GameObject::doCollision(GameObject* _GO)
{
	if (_GO->getPhysicsBody()->getHB() && getPhysicsBody()->getHB() && !_GO->destroyed && !_GO->destroying)
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

void GameObject::initiateDestruction(int destrType, vec3 directionOutwards, float upwardsRatio)
{
	TypeOfDestr = destrType;
	DirOfDestr = directionOutwards;
	destroying = true;
	goUP = upwardsRatio;
	//getPhysicsBody()->getHB()->enabled = false;
	//std::cout << getName() << std::endl;
	//std::cout << goUP << std::endl;
	//std::cout << DirOfDestr << std::endl;
}

void GameObject::setInitials(vec3 iPos, vec3 iRot, vec3 iScale)
{
	initialPosition = iPos;
	initialRotation = iRot;
	initialScale = iScale;

	getPhysicsBody()->getHB()->enabled = true;
}

void GameObject::resetToInitials()
{
	setLocalPos(initialPosition);
	setLocalRot(initialRotation);
	setScale(initialScale);

	destroying = false;
	destroyed = false;

	getPhysicsBody()->reset();
	timeDestroying = 0.f;

	HIDE = false;

	DestructionMat = mat4f::identity();
}

void GameObject::DestructionSequence(float dt)
{
	if (destroying && !destroyed)
	{
		//std::cout << "INSIDE!" << std::endl;

		timeDestroying += dt;

		if (timeDestroying > maxDestrTime)
		{
			destroyed = true;
			HIDE = true;
			needsUpdate = false;

			//std::cout << "GONE!" << std::endl;
		}
		else
		{
			vec3 newPos;
			vec3 newRot;
			vec3 newScale;

			if (TypeOfDestr == 0)
			{
				float LORP = timeDestroying / maxDestrTime;
				float GOUP = goUP;
				float force = 11.f;

				newPos = -DirOfDestr * LORP * GOUP + vec3(0.f, 1.f - (1.f - LORP) * (1.f - LORP), 0.f) * (1.f - GOUP);
				newPos *= force;
				//if (newPos.y < 0.f)
				//	newPos.y = 0.f;
				newRot = vec3(0.f);
				newScale = vec3(sin((1.f - LORP) * PI * 0.75f) / sin(PI * 0.75f));

				//std::cout << getName() << ": " << newPos << ", " << newRot << ", " << newScale << std::endl;
			}

			mat4 rx = rx.rotatex(degrees(newRot.x));
			mat4 ry = ry.rotatey(degrees(newRot.y));
			mat4 rz = rz.rotatez(degrees(newRot.z));

			// Note: pay attention to rotation order, ZYX is not the same as XYZ
			mat4 mRot = rz * ry * rx;

			// Create translation matrix
			mat4 mTran = mTran.translate(newPos);

			// Create scale matrix
			mat4 mScale = mScale.scale(newScale);

			// Combine all above transforms into a single matrix
			DestructionMat = mTran * mRot * mScale;

			//std::cout << DestructionMat << std::endl;
		}
	}
}

