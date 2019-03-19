#include "Weapon.h"
//#include <iostream>

Weapon::Weapon()
{
	TT = TYPE_Weapon;
}

Weapon::~Weapon()
{
}

bool Weapon::tailoredCollision(GameObject * _GO)
{
	if (_GO->getPhysicsBody()->getHB() && Impact)
	{
		//std::cout << flingHitbox << std::endl;
		if (_GO->getPhysicsBody()->getHB()->enabled && flingHitbox > 0)
		{
			if (_GO->getPhysicsBody()->getHB()->dynamic)
			{

			}
			else
			{
				//std::cout << "HERE!" << std::endl;
				if (Impact->collidesWith(_GO->getPhysicsBody()->getHB(), getLocalToWorld(), _GO->getLocalToWorld()))
				{
					//std::cout << "YO" << std::endl;
					vec3 centerToCollision = (Impact->closestPoint - getLocalToWorld().translation()) / Impact->maxRad;
					vec3 outward = normalize(mat3(inverse(_GO->getLocalToWorld())) * Impact->outDir);

					centerToCollision.y = 0.f;
					outward.y = 0.f;

					_GO->initiateDestruction(0, outward, length(centerToCollision), ownedPlayer);
					//_GO->DirOfDestr = outward;
					//_GO->TypeOfDestr = getDest();
					//
					return true;
				}
			}
		}
	}

	return false;
}

void Weapon::update(float dt)
{
	if (dt > 0)
	{
		timeActive += dt;
		otherUpdates(dt);

		if (!timeToDie)
		{
			wFrame* _W0 = getFrameBefore(timeActive);
			wFrame* _W1 = getFrameAt(timeActive);
			wFrame* _W2 = getFrameAfter(timeActive);
			wFrame* _W3 = getFrame2After(timeActive);

			vec3 POS_0;
			vec3 POS_1;
			vec3 POS_2 = _W2->position;
			vec3 POS_3;

			vec3 ROT_0;
			vec3 ROT_1;
			vec3 ROT_2 = _W2->rotationAngles;
			vec3 ROT_3;

			vec3 SCALE_0;
			vec3 SCALE_1;
			vec3 SCALE_2 = _W2->scale;
			vec3 SCALE_3;

			//std::cout << _W2->weapMesh;
			setMesh(_W2->weapMesh);
			setMaterial(_W2->weapTex);
			setShaderProgram(_W2->SP);

			if (_W1)
			{
				POS_1 = _W1->position;
				ROT_1 = _W1->rotationAngles;
				SCALE_1 = _W1->scale;
			}
			else
			{
				POS_1 = vec3(0, 0, 0);
				ROT_1 = vec3(0, 0, 0);
				SCALE_1 = vec3(1, 1, 1);
			}

			if (_W0)
			{
				POS_0 = _W0->position;
				ROT_0 = _W0->rotationAngles;
				SCALE_0 = _W0->scale;
			}
			else
			{
				POS_0 = POS_1;
				ROT_0 = ROT_1;
				SCALE_0 = SCALE_1;
			}

			if (_W3)
			{
				POS_3 = _W3->position;
				ROT_3 = _W3->rotationAngles;
				SCALE_3 = _W3->scale;
			}
			else
			{
				POS_3 = _W2->position;
				ROT_3 = _W2->rotationAngles;
				SCALE_3 = _W2->scale;
			}

			float tT = _W2->tFunc((timeActive - _W2->timeOf) / _W2->duration);
			if (_W2->MT == wFrame::MutationType::LINEAR)
			{
				vec3 newPos = lerp(POS_1, POS_2, tT);
				vec3 newRot = lerp(ROT_1, ROT_2, tT);
				vec3 newScale = lerp(SCALE_1, SCALE_2, tT);

				setLocalPos(newPos);
				setLocalRot(newRot);
				setScale(newScale);
			}
			else if (_W2->MT == wFrame::MutationType::CATMULL)
			{
				vec3 newPos, newRot, newScale;
				newPos.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(POS_0.x, POS_1.x, POS_2.x, POS_3.x));
				newPos.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(POS_0.y, POS_1.y, POS_2.y, POS_3.y));
				newPos.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(POS_0.z, POS_1.z, POS_2.z, POS_3.z));

				newRot.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(ROT_0.x, ROT_1.x, ROT_2.x, ROT_3.x));
				newRot.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(ROT_0.y, ROT_1.y, ROT_2.y, ROT_3.y));
				newRot.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(ROT_0.z, ROT_1.z, ROT_2.z, ROT_3.z));

				newScale.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(SCALE_0.x, SCALE_1.x, SCALE_2.x, SCALE_3.x));
				newScale.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(SCALE_0.y, SCALE_1.y, SCALE_2.y, SCALE_3.y));
				newScale.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(SCALE_0.z, SCALE_1.z, SCALE_2.z, SCALE_3.z));

				setLocalPos(newPos);
				setLocalRot(newRot);
				setScale(newScale);
			}
			//otherUpdates(dt);
		}
		else
		{
			
		}
	}
	Transform::update(dt);
}

void Weapon::draw()
{
	if (getMesh() && getMaterial())
	{
		getMaterial()->bind();
		getMaterial()->sendUniforms();
		getShader()->sendUniform("uModel", getLocalToWorld());
		int i = 0;
		for (Texture* texture : *getTextures())
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		getMesh()->draw();
		for (Texture* texture : *getTextures())
		{
			texture->unbind(--i);
		}
	}
}

mat4 Weapon::getLocalToWorld() const
{
	return worldLocation * Transform::getLocalToWorld();
}

float wFrame::tFunc(float t)
{
	if (LT == wFrame::LerpType::LT_LINEAR)
		return t;
	else if (LT == wFrame::LerpType::LT_SQUARE)
		return t * t;
	else if (LT == wFrame::LerpType::LT_ONE_MINUS_SQUARE)
		return 1.f - pow(1.f - t, 2);
	else if (LT == wFrame::LerpType::LT_HIGH_POWER)
		return 1.f - pow(1.f - t, 6);
	else if (LT == wFrame::LerpType::LT_SINUSOIDAL)
		return pow(sin(t * 3.1415926535f * 0.5f), 2);
	else
		return t;
	//return Attached->frameWarp(t);
}
