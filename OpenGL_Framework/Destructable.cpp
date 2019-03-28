#include "Destructable.h"
#include <iostream>

Destructable::Destructable()
{
	TT = TransformType::TYPE_Destructable;
}

Destructable::~Destructable()
{
}

void Destructable::setBob(float bob)
{
	randomBob = bob;
}

float Destructable::getBob()
{
	return randomBob;
}

void Destructable::draw()
{
	if (getMesh() && getMaterial() && !HIDE)
	{
		getMaterial()->bind();
		getMaterial()->sendUniforms();
		getShader()->sendUniform("uModel", getLocalToWorld() * DestructionMat);
		getShader()->sendUniform("bob", randomBob);
		getShader()->sendUniform("sway", getWorldSwing());
		int i = 0;
		for (Texture* texture : *(getTextures()))
		{
			texture->bind(i++);
		}
		//if (mesh->amntOfSpace > 0)
		getMesh()->draw();
		for (Texture* texture : *(getTextures()))
		{
			texture->unbind(--i);
		}
	}
}

void Destructable::update(float dt)
{
	if (swingTime > 0)
	{
		swingTime -= dt;

		swingPoint = bezierPoint();
	}
	else
	{
		swingPoint = vec3();

		swingTime = 0;

		needsUpdate = false;
	}

	Transform::update(dt);
}

vec3 Destructable::bezierPoint()
{
	float sqswing = swingTime * swingTime / (maxSwingTime * maxSwingTime);
	vec3 stop1 = lerp(vec3(), swingPoint1, sqswing);
	vec3 stop2 = lerp(swingPoint1, swingPoint2, sqswing);
	vec3 stop3 = lerp(swingPoint2, vec3(), sqswing);

	stop1 = lerp(stop1, stop2, sqswing);
	stop2 = lerp(stop2, stop3, sqswing);

	return lerp(stop1, stop2, sqswing);
}
