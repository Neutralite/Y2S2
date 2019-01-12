#pragma once

#include "Jmath.h"
#include <string>

class Transform
{
public:
	float m_pScale;

	float m_pRotX = 0.0f, 
		m_pRotY = 0.0f, 
		m_pRotZ = 0.0f; // local rotation angles

	vec3 m_pLocalPosition;

	// TODO: Compute these matrices in the Update function
	mat4 m_pLocalRotation;
	mat4 m_pLocalToWorldMatrix;

	Transform();
	~Transform();

	void setPosition(vec3 newPosition);
	void setRotationAngle(vec3 newAngle);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	vec3 getPosition();
	vec3 getRotationAngle();
	float getRotationAngleX();
	float getRotationAngleY();
	float getRotationAngleZ();
	float getScale();

	void setRotationAngle(mat4 randomRot);

	mat4 getLocalToWorldMatrix();
	mat4 getLocalToWorldMatrixInverseScale();

	// TODO: Implement this update function
	virtual void update(float dt);	
	
	// Other Properties
	std::string name;
	vec4 color; 
};
