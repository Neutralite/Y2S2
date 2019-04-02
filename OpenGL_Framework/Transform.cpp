#include "Transform.h"
#include "IO.h"
#include <iostream>

Transform::Transform()
	: m_pLocalScale(1.0f), color(vec4(1.0f)), m_pParent(nullptr)
{
	TT = TransformType::TYPE_Transform;
}

Transform::~Transform()
{
	Sound::Stop(sound);
}

void Transform::addChild(Transform * newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->m_pParent = this; 
	}
}

void Transform::removeChild(Transform * rip)
{
	for (unsigned int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) 
		{
			m_pChildren.erase(m_pChildren.begin() + i);
			--i;
		}
	}
}

void Transform::removeChild(unsigned int ripChild)
{
	if (ripChild < m_pChildren.size())
		m_pChildren.erase(m_pChildren.begin() + ripChild);
}

void Transform::removeChild(std::string ripChild)
{
	for (unsigned int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i]->getName() == ripChild)
		{
			m_pChildren.erase(m_pChildren.begin() + i);
			--i;
		}
	}
}

void Transform::setLocalPos(vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setLocalRotX(float newAngle)
{
	m_pLocalRotationEuler.x = newAngle;
}

void Transform::setLocalRotY(float newAngle)
{
	m_pLocalRotationEuler.y = newAngle;
}

void Transform::setLocalRotZ(float newAngle)
{
	m_pLocalRotationEuler.z = newAngle;
}

void Transform::setLocalRot(vec3 newAngle)
{
	m_pLocalRotationEuler = newAngle;
}

void Transform::setScale(vec3 newScale)
{
	m_pLocalScale = newScale;
}

void Transform::setScale(float newScale)
{
	m_pLocalScale = vec3(newScale);
}

void Transform::rotateBy(float angU, vec3 axis)
{
	m_pForcedRotation = mat4f::rotate(degrees(angU), axis) * m_pForcedRotation;
}

void Transform::resetAlternateRotate()
{
	m_pForcedRotation = mat4f::identity();
}

mat4 Transform::getLocalToWorld() const
{
	return m_pLocalToWorld;
}

vec3 Transform::getWorldPos() const
{
	if (m_pParent)
		return vec3(m_pParent->getLocalToWorld() * vec4(m_pLocalPosition, 1.0f));
	else
		return m_pLocalPosition;
}

mat4 Transform::getWorldRot() const
{
	if (m_pParent)
		return m_pParent->getWorldRot() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

vec3 Transform::getLocalScale() const
{
	return m_pLocalScale;
}

vec3 Transform::getLocalPos() const
{
	return m_pLocalPosition;
}

mat4 Transform::getLocalRot() const
{
	return m_pLocalRotation;
}

vec3 Transform::getLocalEuler() const
{
	return m_pLocalRotationEuler;
}

vec3 Transform::getWorldSwing() const
{
	if (m_pParent)
		return swingPoint + m_pParent->getWorldSwing();
	return swingPoint;
}

std::vector<Transform*> Transform::getChildren() const
{
	return m_pChildren;
}

Transform * Transform::getParent() const
{
	return m_pParent;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	mat4 rx = rx.rotatex(degrees(m_pLocalRotationEuler.x));
	mat4 ry = ry.rotatey(degrees(m_pLocalRotationEuler.y));
	mat4 rz = rz.rotatez(degrees(m_pLocalRotationEuler.z));

	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran = tran.translate(m_pLocalPosition);

	// Create scale matrix
	mat4 scale = scale.scale(m_pLocalScale);

	// Combine all above transforms into a single matrix
	m_pLocalTransform = tran * m_pLocalRotation * m_pForcedRotation * scale;

	mat4 subStep = mat4::identity();
	if (m_pParent)
	{
		m_pLocalToWorld = m_pParent->getLocalToWorld() * m_pLocalTransform;
		subStep = subStep.translate(getWorldSwing() * m_pLocalToWorld.translation().y);
		//if (length(getWorldSwing()) > 0.001f)
		//	std::cout << subStep << std::endl << std::endl;
		m_pLocalToWorld = subStep * m_pParent->getLocalToWorld() * m_pLocalTransform;
		//if (length(getWorldSwing()) > 0.001f)
		//	std::cout << m_pLocalToWorld << std::endl << std::endl;
	}
	else
	{
		m_pLocalToWorld = m_pLocalTransform;
	}

	DestructionSequence(dt);

	// Update children
	for (unsigned int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);

	if (sound)
		Sound::SetPosition(sound, toFV(getWorldPos()));
}

FMOD_VECTOR Transform::toFV(vec3 _V)
{
	FMOD_VECTOR FV = { _V.x, _V.y, _V.z };
	return FV;
}

void Transform::draw()
{
	SAT_DEBUG_LOG_WARNING("DRAWING TRANSFORM INSTEAD OF GAMEOBJECT");
}

void Transform::render()
{
	SAT_DEBUG_LOG_WARNING("RENDERING FROM NON-CAMERA OBJECT");
}

void Transform::dynamicDraw()
{
	SAT_DEBUG_LOG_WARNING("DRAWING TRANSFORM INSTEAD OF GAMEOBJECT");
}

void Transform::attachFrameBuffer(Framebuffer* fb)
{
	SAT_DEBUG_LOG_WARNING("SETTING FRAMEBUFFER ON TRANSFORM");
}

Framebuffer* Transform::getFrameBuffer()
{
	SAT_DEBUG_LOG_WARNING("GETTING FRAMEBUFFER FORM TRANSFORM");
	return nullptr;
}

void Transform::setRenderList(std::vector<Transform*> objects)
{
	SAT_DEBUG_LOG_WARNING("SETTING RENDERING FROM NON-CAMERA OBJECT");
}

void Transform::addToRenderList(std::vector<Transform*> objects)
{
	SAT_DEBUG_LOG_WARNING("ADDING TO RENDERING FROM NON-CAMERA OBJECT");
}

std::string Transform::getName()
{
	return name;
}

void Transform::setName(std::string _NAME)
{
	name = _NAME;
}

vec3 Transform::bezierPoint()
{
	//float sqswing = swingTime * swingTime / (maxSwingTime * maxSwingTime);
	//vec3 stop1 = lerp(vec3(), swingPoint1, sqswing);
	//vec3 stop2 = lerp(swingPoint1, swingPoint2, sqswing);
	//vec3 stop3 = lerp(swingPoint2, vec3(), sqswing);
	//
	//stop1 = lerp(stop1, stop2, sqswing);
	//stop2 = lerp(stop2, stop3, sqswing);
	//
	//return lerp(stop1, stop2, sqswing);
	return vec3();
}

void Transform::DestructionSequence(float dt)
{
}
