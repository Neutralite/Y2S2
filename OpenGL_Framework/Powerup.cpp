#include "Powerup.h"
#include "ResourceManager.h"

Powerup::Powerup()
{
	TT = TYPE_Powerup;

	randomColorOffset = (float)(rand() % 628);
	randomColorOffset *= 0.01f;
	timeAccum = 0.f;

	setMesh(rm::getMesh("TESTING_BASE_PLATE"));
	//std::cout << "-----" << std::endl;
	setMaterial(rm::getMaterial("POWERUP"));
	//std::cout << "!!!!!" << std::endl;

	destrPoints = 0;
}

Powerup::~Powerup()
{

}

void Powerup::setPower(pType PT)
{
	_P = PT;
	if (PT == MINE)
	{
		GameObject* _W = rm::getCloneOfObject("MINE_MODEL");
		_W->setScale(4.f);
		addChild(_W);
	}
	else if (PT == HAMMER)
	{
		GameObject* _W = rm::getCloneOfObject("HAMMER_MODEL");
		_W->setScale(2.5f);
		addChild(_W);
	}
	else if (PT == AXE)
	{
		GameObject* _W = rm::getCloneOfObject("AXE_MODEL");
		_W->setScale(3.f);
		_W->setLocalRotX(-90.f);
		addChild(_W);
	}

	randomColorOffset = (float)(rand() % 628);
	randomColorOffset *= 0.01f;
	timeAccum = 0.f;
	setLocalRotY(randomColorOffset * 180.f / 3.14159f);
	setScale(0.3f);
}

void Powerup::draw()
{
	if (getMesh() && getMaterial() && !HIDE)
	{
		getMaterial()->bind();
		getMaterial()->sendUniforms();
		getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		getShader()->sendUniform("cShift", randomColorOffset + timeAccum);
		int i = 0;
		for (Texture* texture : *(getTextures()))
		{
			texture->bind(i++);
		}
		getMesh()->draw();
		for (Texture* texture : *(getTextures()))
		{
			texture->unbind(--i);
		}
	}
}

void Powerup::update(float dt)
{
	if (getChildren().size() > 0)
	{
		setLocalRotY(getLocalEuler().y + 60.f * dt);
		setLocalPos(vec3(getLocalPos().x, fmod(0.003f * timeAccum, 0.1f) + 0.001f, getLocalPos().z));
		timeAccum += dt;
		getChildren().at(0)->setLocalPos(vec3(0, 4.f + cos(timeAccum * 2.5f + 0.1f) / (1.f - pow(2.f, -pow(timeAccum * 2.5f + 0.1f, 2))), 0));
		if (!getPhysicsBody()->getHB() && timeAccum > 1.57f)
		{
			getPhysicsBody()->setHB(rm::getHitbox("GRASS_HITBOX"));
		}
		Transform::update(dt);
	}
}

vec3 Powerup::bezierPoint()
{
	return vec3(0, 0, 0);
}
