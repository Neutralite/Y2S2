#include "Transform.h"

Transform::Transform()
	: m_pScale(1.0f),
	color(vec4(1.0f)), m_pRotX(0), m_pRotY(0), m_pRotZ(0), m_pLocalPosition(vec3(0, 0, 0)), m_pLocalToWorldMatrix(mat4::Identity)
{
}

Transform::~Transform() {}

void Transform::setPosition(vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::rotateAroundAxis(vec3 axis, float degrees)
{
	vec3 oPos = getPosition();
	m_pLocalToWorldMatrix.SetTranslation(vec3(0, 0, 0));
	m_pLocalToWorldMatrix.Rotate(axis, degrees);
	m_pLocalToWorldMatrix.SetTranslation(oPos);
}

void Transform::setRotationAngle(vec3 newAngle)
{
	m_pRotX = newAngle.x;
	m_pRotY = newAngle.y;
	m_pRotZ = newAngle.z;
}

void Transform::setRotationAngleX(float newAngle)
{
	m_pRotX = newAngle;
}

void Transform::setRotationAngleY(float newAngle)
{
	m_pRotY = newAngle;
}

void Transform::setRotationAngleZ(float newAngle)
{
	m_pRotZ = newAngle;
}

void Transform::setScale(float newScale)
{
	m_pScale = newScale;
}

vec3 Transform::getPosition()
{
	return m_pLocalPosition;
}

vec3 Transform::getRotationAngle()
{
	return vec3(m_pRotX, m_pRotY, m_pRotZ);
}

float Transform::getRotationAngleX()
{
	return m_pRotX;
}

float Transform::getRotationAngleY()
{
	return m_pRotY;
}

float Transform::getRotationAngleZ()
{
	return m_pRotZ;
}

float Transform::getScale()
{
	return m_pScale;
}

void Transform::setRotationAngle(mat4 randomRot)
{
	vec4 R1 = randomRot * vec4(1, 0, 0, 0);
	vec4 R2 = randomRot * vec4(0, 1, 0, 0);
	vec4 R3 = randomRot * vec4(0, 0, 1, 0);

	float M1 = R1.x;
	float M4 = R1.y;
	float M7 = R1.z;
	float M2 = R2.x;
	float M5 = R2.y;
	float M8 = R2.z;
	float M3 = R3.x;
	float M6 = R3.y;
	float M9 = R3.z;

	float RX, RY, RZ;

	if (M7 == 0.f)
	{
		RY = 0.f;

		RZ = ToDegrees(acos(M1));
		if (asin(M4) < 0)
			RZ *= -1.f;

		RX = ToDegrees(acos(M9));
		if (asin(M8) < 0)
			RX *= -1.f;
	}
	else if (abs(M7) == 1.f)
	{
		RY = -ToDegrees(asin(M7));

		RZ = 0.f;

		RX = ToDegrees(acos(M5));
		if (asin(M6) > 0)
			RX *= -1.f;
	}
	else
	{
		float s2a = 2.f * M1 * M4 / (1 - M7 * M7);
		float c2a = (M1 * M1 - M4 * M4) / (1 - M7 * M7);
		float s2b = 2.f * M9 * M8 / (1 - M7 * M7);
		float c2b = (M9 * M9 - M8 * M8) / (1 - M7 * M7);

		RZ = 0.5f * ToDegrees(acos(c2a));
		if (asin(s2a) < 0)
			RZ *= -1.f;

		RX = 0.5f * ToDegrees(acos(c2b));
		if (asin(s2b) < 0)
			RX *= -1.f;

		float rawCZ = cos(0.5f * acos(c2a));
		float rawSZ = sin(0.5f * acos(c2a));

		if (abs(M1) > abs(M4))
		{
			RY = ToDegrees(acos(M1 / rawCZ));
			if (asin(M7) > 0)
				RY *= -1.f;
		}
		else
		{
			RY = ToDegrees(acos(M4 / rawSZ));
			if (asin(M7) > 0)
				RY *= -1.f;
		}

		if (cos(ToRadians(RX)) * cos(ToRadians(RY)) * M9 < 0.f)
		{
			RX += 180.f;
			if (RX > 180.f)
				RX -= 360.f;
		}
		if (cos(ToRadians(RZ)) * cos(ToRadians(RY)) * M1 < 0.f)
		{
			RZ += 180.f;
			if (RZ > 180.f)
				RZ -= 360.f;
		}
	}

	m_pRotX = RX;
	m_pRotY = RY;
	m_pRotZ = RZ;
}

mat4 Transform::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

mat4 Transform::getLocalToWorldMatrixInverseScale()
{
	mat4 scale;
	scale.Scale(1.f / m_pScale);

	return m_pLocalToWorldMatrix * scale * scale;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix

	mat4 rx; 
	mat4 ry; 
	mat4 rz; 

	rx.RotateX(m_pRotX);
	ry.RotateY(m_pRotY);
	rz.RotateZ(m_pRotZ);
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran; 
	tran.Translate(m_pLocalPosition);

	// Create scale matrix
	mat4 scale; 
	scale.Scale(m_pScale);

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalToWorldMatrix = tran * m_pLocalRotation * scale;
}