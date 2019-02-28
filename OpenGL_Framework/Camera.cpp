#include "Camera.h"
#include "ResourceManager.h"
#include "IO.h"
#include <algorithm>

Camera* activeCamera;
vec3 activeCameraPosition;
UniformBuffer Camera::m_pUBO;

Camera::Camera() : Camera(ProjectionType::Perspective)
{
	TT = TransformType::TYPE_Camera;
}

Camera::Camera(ProjectionType projType)
{
	init();
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f); 
	else
		orthographic(-10, 10, -10, 10, -100, 100);
	TT = TransformType::TYPE_Camera;
}

Camera::~Camera()
{
	m_pUBO.unload();
}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pProjectionType = ProjectionType::Perspective;
	m_pProjection = mat4::perspective(
		degrees(fovy), aspect, 
		zNear, zFar);

	m_pAspectRatio = aspect;
	m_pNear = zNear;
	m_pFar = zFar;
	m_pFov.y = fovy;
	m_pFov.x = atan(tan(degrees(m_pFov.y).radians() * 0.5f) * aspect) * 2.0f;
	sendUBO();
}

void Camera::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pProjectionType = ProjectionType::Orthographic;
	m_pProjection = mat4f::ortho(
		left, right, 
		bottom, top,
		zNear, zFar);

	m_pOrthoSize = vec4(left, right, bottom, top);
	m_pAspectRatio = (right - left) / (top - bottom);
	m_pNear = zNear;
	m_pFar = zFar;
	sendUBO();
}

void Camera::init()
{
	if (!m_pUBO.isInit())
	{
		m_pUBO.allocateMemory(sizeof(mat4) * 3 + sizeof(vec4));
	}
}

mat4 Camera::getView() const
{
	return m_pViewMatrix;
}

mat4 Camera::getViewProjection() const
{
	return m_pProjection * m_pViewMatrix;
}

mat4 Camera::getProjection() const
{
	return m_pProjection;
}

mat4* Camera::getViewProjectionPtr() 
{
	return &m_pProjection;
}

void Camera::update(float dt)
{
	Transform::update(dt); // original inherited update function
	m_pViewMatrix = inverse(m_pLocalToWorld);
	sendUBO();
	
	cull();
	sort();
}

void Camera::sendUBO() const
{
	m_pUBO.sendMatrix(getProjection(), 0);
	m_pUBO.sendMatrix(getView(), sizeof(mat4));
	m_pUBO.sendMatrix(gmtk::inverse(getProjection()), sizeof(mat4) * 2);
	m_pUBO.sendVector(getLocalToWorld().translation(), sizeof(mat4) * 3);
}

void Camera::draw()
{

}

void Camera::render()
{
	SAT_ASSERT(m_pUBO.isInit() == true, "Camera's uniform buffer has not been initialized!");
	m_pUBO.bind(0);
	
	if (m_pFB != nullptr)
	{
		m_pFB->setViewport();
		m_pFB->bind();
		clear();
	}

	activeCamera = this;
	activeCameraPosition = getLocalToWorld().translation();
	for (Transform* object : cullList)
	{
		object->draw();
	}

	if (m_pFB != nullptr)
	{
		m_pFB->unbind();
	}
}

void Camera::clear()
{
	GLbitfield clearFlag = 0;
	switch (m_pClearFlag)
	{
	case ClearFlag::Skybox:
		clearFlag |= GL_DEPTH_BUFFER_BIT;
		break;
	case ClearFlag::SolidColor:
		clearFlag |= GL_COLOR_BUFFER_BIT;
	case ClearFlag::DepthOnly:
		clearFlag |= GL_DEPTH_BUFFER_BIT;
		break;
	case ClearFlag::NoClear:
		break;
	}

	if(m_pClearFlag == ClearFlag::SolidColor)
		glClearColor(m_pClearColor.r, m_pClearColor.g, m_pClearColor.b, m_pClearColor.a);
	glClear(clearFlag);

	if (m_pClearFlag == ClearFlag::Skybox)
	{
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		m_pUBO.sendMatrix(getLocalToWorld(), sizeof(mat4));
		SAT_ASSERT(m_pSkybox != nullptr, "Camera has no skybox!");
		m_pSkybox->draw();
		m_pUBO.sendMatrix(getView(), sizeof(mat4));
		//glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}	
}

void Camera::cull()
{
	cullList.clear();
	if (cullingActive)
	{
		if (m_pProjectionType == Perspective)
		{
			// assuming aspect ratio is bigger on X than Y
			for (Transform* object : objectList)
			{
				vec3 direction = normalize(this->m_pLocalToWorld.translation() - object->m_pLocalToWorld.translation());
				if (dot(direction, this->m_pLocalToWorld.forward()) > cos(m_pFov.x * 0.5f * 1.4f))
				{
					cullList.push_back(object);
				}
			}
		}
		else
		{
			for (Transform* object : objectList)
			{
				cullList.push_back(object);
			}
		}
	}
	else
	{
		for (Transform* object : objectList)
		{
			cullList.push_back(object);
		}
	}
}

struct
{
	bool operator()(Transform* a, Transform* b) const
	{
		return (distance(a->getLocalToWorld().translation(), activeCameraPosition) < distance(b->getLocalToWorld().translation(), activeCameraPosition));
	}
} customSort;

void Camera::sort()
{
	std::sort(objectList.begin(), objectList.end(), customSort);
}

void Camera::attachFrameBuffer(Framebuffer * fb)
{
	m_pFB = fb;
}

Framebuffer* Camera::getFrameBuffer()
{
	return m_pFB;
}

void Camera::setRenderList(std::vector<Transform*> objects)
{
	objectList = objects;
}

void Camera::addToRenderList(std::vector<Transform*> objects)
{
	objectList = ResourceManager::Transforms;
}