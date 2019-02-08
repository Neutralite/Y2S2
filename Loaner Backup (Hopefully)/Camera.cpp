#include "Camera.h"

#include <random>

Camera::Camera()
{
}

Camera::Camera(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f); 
	else
		orthographic(-10, 10, -10, 10, -100, 100);
}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	projectionType = ProjectionType::Perspective;
	projection = mat4::PerspectiveProjection(
		fovy, aspect, 
		zNear, zFar);
	FOVy = fovy;
	ASPECT = aspect;
}

void Camera::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionType = ProjectionType::Orthographic;
	projection = mat4::OrthographicProjection(
		left, right, 
		bottom, top,
		zNear, zFar);
}

mat4 Camera::getView() const
{
	return m_pLocalToWorldMatrix.GetInverse();
}

mat4 Camera::getProjection() const
{
	return projection;
}