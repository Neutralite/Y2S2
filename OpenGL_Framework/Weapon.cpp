#include "Weapon.h"

Weapon::Weapon()
{
	TT = TYPE_Weapon;
}

Weapon::~Weapon()
{
}

void Weapon::update(float dt)
{
	if (dt > 0)
	{
		timeActive += dt;
		wFrame* _W0 = getFrameBefore(timeActive);
		wFrame* _W1 = getFrameAt(timeActive);
		wFrame* _W2 = getFrameAfter(timeActive);
		wFrame* _W3 = getFrame2After(timeActive);
		if (_W1 && _W2)
		{
			float tT = _W1->tFunc((timeActive - _W1->timeOf) / _W1->duration);
			if (_W1->MT == wFrame::MutationType::LINEAR)
			{
				vec3 newPos = lerp(_W1->position, _W2->position, tT);
				vec3 newRot = lerp(_W1->rotationAngles, _W2->rotationAngles, tT);
				vec3 newScale = lerp(_W1->scale, _W2->scale, tT);

				setLocalPos(newPos);
				setLocalRot(newRot);
				setScale(newScale);
			}
			else if (_W1->MT == wFrame::MutationType::CATMULL)
			{
				vec3 newPos, newRot, newScale;
				newPos.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->position.x, _W1->position.x, _W2->position.x, _W3->position.x));
				newPos.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->position.y, _W1->position.y, _W2->position.y, _W3->position.y));
				newPos.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->position.z, _W1->position.z, _W2->position.z, _W3->position.z));

				newRot.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->rotationAngles.x, _W1->rotationAngles.x, _W2->rotationAngles.x, _W3->rotationAngles.x));
				newRot.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->rotationAngles.y, _W1->rotationAngles.y, _W2->rotationAngles.y, _W3->rotationAngles.y));
				newRot.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->rotationAngles.z, _W1->rotationAngles.z, _W2->rotationAngles.z, _W3->rotationAngles.z));

				newScale.x = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->scale.x, _W1->scale.x, _W2->scale.x, _W3->scale.x));
				newScale.y = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->scale.y, _W1->scale.y, _W2->scale.y, _W3->scale.y));
				newScale.z = dot(vec4(pow(tT, 3), pow(tT, 2), tT, 1.f), mat4::catmull() * vec4(_W0->scale.z, _W1->scale.z, _W2->scale.z, _W3->scale.z));

				setLocalPos(newPos);
				setLocalRot(newRot);
				setScale(newScale);
			}
			otherUpdates(dt);
		}
		else
		{
			timeToDie = true;
		}
	}
	Transform::update(dt);
}

float wFrame::tFunc(float t)
{
	return 0.0f;
}
