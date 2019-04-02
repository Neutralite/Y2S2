#include "Camera.h"
#include "ResourceManager.h"
#include "IO.h"
#include <algorithm>
#include <iostream>

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
	//m_pUBO.unload();
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
	if (PLAYER)
	{
		PLAYER->pos = toFV(getWorldPos());
		PLAYER->forward = toFV(getLocalToWorld().forward());
		PLAYER->up = toFV(getLocalToWorld().up());
	}
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

void Camera::render(bool useFB)
{
	SAT_ASSERT(m_pUBO.isInit() == true, "Camera's uniform buffer has not been initialized!");
	m_pUBO.bind(0);
	
	if (m_pFB != nullptr && useFB)
	{
		//m_pFB->setViewport();
		m_pFB->bind();
		clear();
	}

	activeCamera = this;
	activeCameraPosition = getLocalToWorld().translation();
	for (Transform* object : cullList)
	{
		object->draw();
	}

	if (m_pFB != nullptr && useFB)
	{
		m_pFB->unbind();
	}
}

void Camera::renderToFB(Framebuffer * _FB, bool clearIt)
{
	SAT_ASSERT(m_pUBO.isInit() == true, "Camera's uniform buffer has not been initialized!");
	m_pUBO.bind(0);

	if (_FB != nullptr)
	{
		//_FB->setViewport();
		_FB->bind();
		if (clearIt)
			clear();
	}

	activeCamera = this;
	activeCameraPosition = getLocalToWorld().translation();
	for (Transform* object : cullList)
	{
		object->draw();
	}

	if (_FB != nullptr)
	{
		_FB->unbind();
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
		for (Transform* object : objectList)
		{
			if (object->doCull && object->TT != TransformType::TYPE_Text)
			{
				//std::cout << "THERE WAS" << std::endl;
				vec4 center = m_pViewMatrix * object->getLocalToWorld() * vec4(object->tCenter, 1.f);
				
				vec4 T1 = m_pProjection * (center + vec4(object->drawPoint1, 1.f));
				vec4 T2 = m_pProjection * (center + vec4(object->drawPoint2, 1.f));

				//T1 /= T1.w;
				//T2 /= T2.w;

				//std::cout << T1 << ",   " << T2 << " |_|_|_|_| " << object->drawPoint1 << ",   " << object->drawPoint2 << std::endl;

				if (T1.x > -T1.w && T1.y > -T1.w && T2.x < T2.w && T2.y < T2.w)
					cullList.push_back(object);
			}
			else
			{
				//std::cout << "NOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
				cullList.push_back(object);
			}
		}
	}
	else
	{
		//std::cout << "HAHAHAHA" << std::endl;
		for (Transform* object : objectList)
		{
			cullList.push_back(object);
		}
	}
}

void Camera::cullGrass()
{
	cullList.clear();
	if (cullingActive)
	{
		for (Transform* object : objectList)
		{
			if (Destructable* _DS = dynamic_cast<Destructable*> (object))
			{
				if (object->doCull && _DS->getPhysicsBody())
				{
					if (_DS->getPhysicsBody()->getHB())
					{
						if (!_DS->getPhysicsBody()->getHB()->grass)
						{
							//std::cout << "THERE WAS" << std::endl;
							vec4 center = m_pViewMatrix * object->getLocalToWorld() * vec4(object->tCenter, 1.f);

							vec4 T1 = m_pProjection * (center + vec4(object->drawPoint1, 1.f));
							vec4 T2 = m_pProjection * (center + vec4(object->drawPoint2, 1.f));

							//T1 /= T1.w;
							//T2 /= T2.w;

							//std::cout << T1 << ",   " << T2 << " |_|_|_|_| " << object->drawPoint1 << ",   " << object->drawPoint2 << std::endl;

							if (T1.x > -T1.w && T1.y > -T1.w && T2.x < T2.w && T2.y < T2.w)
								cullList.push_back(object);
						}
					}
				}
			}
			else if (object->doCull && object->TT != TransformType::TYPE_Text)
			{
				//std::cout << "THERE WAS" << std::endl;
				vec4 center = m_pViewMatrix * object->getLocalToWorld() * vec4(object->tCenter, 1.f);

				vec4 T1 = m_pProjection * (center + vec4(object->drawPoint1, 1.f));
				vec4 T2 = m_pProjection * (center + vec4(object->drawPoint2, 1.f));

				//T1 /= T1.w;
				//T2 /= T2.w;

				//std::cout << T1 << ",   " << T2 << " |_|_|_|_| " << object->drawPoint1 << ",   " << object->drawPoint2 << std::endl;

				if (T1.x > -T1.w && T1.y > -T1.w && T2.x < T2.w && T2.y < T2.w)
					cullList.push_back(object);
			}
			else
			{
				//std::cout << "NOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
				cullList.push_back(object);
			}
		}
	}
	else
	{
		//std::cout << "HAHAHAHA" << std::endl;
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
	//std::sort(objectList.begin(), objectList.end(), customSort);
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

void Camera::giveNewOrthoRatio(float _ASPECT)
{
	m_pOrthoSize.x = m_pOrthoSize.z * _ASPECT;
	m_pOrthoSize.y = m_pOrthoSize.w * _ASPECT;

	orthographic(m_pOrthoSize.x, m_pOrthoSize.y, m_pOrthoSize.z, m_pOrthoSize.w, m_pNear, m_pFar);
}

void Camera::giveNewPersRatio(float _ASPECT)
{
	perspective(m_pFov.y, _ASPECT, m_pNear, m_pFar);
}
