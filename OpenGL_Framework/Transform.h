#pragma once

#include <gmtk/gmtk.h>
#include <vector>
#include <string>
#include "Framebuffer.h"

class Transform
{
public:
	vec3f m_pLocalScale;
	vec3f m_pLocalRotationEuler;
	vec3f m_pLocalPosition;

	mat4f m_pLocalRotation;
	mat4f m_pForcedRotation;
	
	mat4f m_pLocalTransform;
	mat4f m_pLocalToWorld;

	Transform* m_pParent;
	std::vector<Transform*> m_pChildren;

	Transform();
	virtual ~Transform();

	void addChild(Transform* newChild);
	void removeChild(Transform* ripChild);
	void removeChild(unsigned int ripChild);
	void removeChild(std::string ripChild);

	void setLocalPos(vec3 newPosiiton); // Local Position
	void setLocalRotX(float newAngle); // Local Rotation X
	void setLocalRotY(float newAngle); // Local Rotation Y
	void setLocalRotZ(float newAngle); // Local Rotation Z
	void setLocalRot(vec3 newAngle); // Local Rotation
	void setScale(vec3 newScale); // Local Scale
	void setScale(float newScale); // Local Scale
	void rotateBy(float angU, vec3 axis);
	void resetAlternateRotate();

	virtual mat4 getLocalToWorld() const; // Get Local to World Matrix
	vec3 getWorldPos() const; // Get World Position
	mat4 getWorldRot() const; // Get World Rotation
	vec3 getLocalScale() const; // Get Local Scale
	vec3 getLocalPos() const; // Get Local Position
	mat4 getLocalRot() const; // Get Local Rotation
	vec3 getLocalEuler() const;
	vec3 getWorldSwing() const;

	std::vector<Transform*> getChildren() const;
	Transform* getParent() const;

	virtual void update(float dt);	
	void updateOframes(float dt);
	virtual void draw();
	virtual void render();
	virtual void dynamicDraw();

	virtual void attachFrameBuffer(Framebuffer* fb);
	virtual Framebuffer* getFrameBuffer();

	virtual void setRenderList(std::vector<Transform*> objects);
	virtual void addToRenderList(std::vector<Transform*> objects);

	std::string getName();
	void setName(std::string _NAME);

	enum TransformType
	{
		TYPE_Transform,
		TYPE_GameObject,
		TYPE_BasePlate,
		TYPE_Boundary,
		TYPE_Player,
		TYPE_Destructable,
		TYPE_Light,
		TYPE_Camera,
		TYPE_Text,
		TYPE_Weapon,
		TYPE_Axe,
		TYPE_Hammer,
		TYPE_Mine,
		TYPE_Powerup,
		TYPE_Recolor
	};

	TransformType TT;


	vec3 swingPoint1;
	vec3 swingPoint2;

	float swingTime = 0.f;
	float maxSwingTime = 0.3f;

	virtual vec3 bezierPoint();
	virtual void DestructionSequence(float dt);
	vec3 swingPoint;

	bool doCull = false;
	bool HIDE = false;

	bool destroying = false;
	bool destroyed = false;

	vec3 tCenter;
	vec3 drawPoint1;
	vec3 drawPoint2;

protected:
	// Other Properties
	std::string name;
	vec4f color; 
	mat4f aniMat;
};