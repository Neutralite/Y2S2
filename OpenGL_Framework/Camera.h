#pragma once
#include "Transform.h"
#include "LERP.h"

enum ProjectionType
{
	Perspective,
	Orthographic
};

class Camera : public Transform
{
public:	
	Camera();
	Camera(ProjectionType projType);

	void perspective(
		float fovy, float aspect, 
		float zNear, float zFar);

	void orthographic(
		float left, float right,
		float bottom, float top,
		float zNear, float zFar);

	mat4 getView() const;
	mat4 getProjection() const;

	void camChase(float dt)
	{
		setPosition(LP::LERP(getPosition(), chasePos, pow(0.25f, 60.f * dt)));
		if ((getPosition() - chasePos).Length() < 0.01f)
			setPosition(chasePos);
	}

	void setChasePosition(vec3 _POS)
	{
		chasePos = _POS;
	}

	float FOVy, ASPECT;

private:
	ProjectionType projectionType = ProjectionType::Perspective;
	mat4 projection;
	vec3 chasePos;
};