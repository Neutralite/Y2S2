#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
#include "PhysicsBody.h"

/*
  ////////////////
 // GameObject //
////////////////

The GameObject class is used for easily representing objects in the scene, 
each object gets a reference to a mesh, multiple textures, and a material,
which determines what shader the object is drawn with

*/

class GameObject : public Transform
{
public:
	GameObject();
	GameObject(const std::string &meshFile, const std::string &textureFile); // Not defined
	GameObject(const std::string &meshFile, const std::vector <std::string> &textureFiles); // Not defined
	GameObject(Mesh* _mesh, Texture *_texture);
	GameObject(Mesh* _mesh, std::vector <Texture*>& _textures);
	~GameObject();

	PhysicsBody* getPhysicsBody();
	void setPhysicsBody(PhysicsBody _PB);

	void setMesh(Mesh* _mesh);
	Mesh* getMesh();
	void addTexture(Texture* _texture);
	void setTexture(Texture* _texture);
	void setTextures(std::vector <Texture*>& _textures);
	void setShaderProgram(ShaderProgram* _shaderProgram);
	ShaderProgram* getShader();
	std::vector<Texture*>* getTextures();
	void draw();
	void dynamicDraw();

	void doCollision(GameObject* _GO);

	//VIRTUAL FUNCS FOR CHILDREN
	virtual void setBoundingRadius(float boundingRadius) {};
	virtual float getBoundingRadius() { return 0; };

	virtual vec3 getVelocity();
	virtual vec3 getAcceleration();

	virtual vec3 getAngularVelocity();
	virtual vec3 getAngularAcceleration();

	virtual void applySwing(vec3 P1, vec3 P2, float ratio);

	virtual void initiateDestruction(int destrType, vec3 directionOutwards);
	void setInitials(vec3 iPos, vec3 iRot, vec3 iScale);
	virtual void resetToInitials();

	int mapX;
	int mapY;

	vec3 pushAgainst;
	float repulsionForce = 2000.f;

	bool needsUpdate = false;
	bool hasBeenUpdated = false;

	bool destroying = false;
	bool destroyed = false;
	bool hasInitial = false;

private:
	Mesh* mesh;
	std::vector<Texture*> textures;
	ShaderProgram* material;
	PhysicsBody PB;
protected:
	vec3 initialRotation;
	vec3 initialPosition;
	vec3 initialScale;

	vec3 DirOfDestr;
	int TypeOfDestr;
};