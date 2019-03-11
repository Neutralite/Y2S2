#pragma once
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"
#include "HITBOX2D.h"
#include "Boundary.h"
#include "BasePlate.h"
#include "Destructable.h"
#include "Player.h"
#include "Weapon.h"
//#include "WEAPON_Hammer.h"
#include "WEAPON_Mine.h"
#include "Material.h"

class ResourceManager
{
public:
	static void addEntity(Transform* entity);
	static void addShader(ShaderProgram* shader);
	static void addTexture(Texture* TEX);
	static void addMesh(Mesh* MESH);
	static void addHitbox(Hitbox2D* HB);
	static void addFramebuffer(Framebuffer* FB);
	static void addMaterial(Material* _M);

	static void addEntityINGAME(Transform* entity);

	static std::vector<ShaderProgram*> Shaders;
	static std::vector<Transform*> Transforms;
	static std::vector<Camera*> Cameras;
	static std::vector<Material*> Materials;

	static std::vector<Texture*> allTextures;
	static std::vector<Mesh*> allMeshes;
	static std::vector<GameObject*> allGameObjects;
	static std::vector<Light*> AllLight; //PLUS ULTRA
	static std::vector<Hitbox2D*> allHitboxes;
	static std::vector<Framebuffer*> allFramebuffers;
	static std::vector<Weapon*> allWeapons;

	static std::vector<Transform*> TransformsINGAME;
	static std::vector<Camera*> CamerasINGAME;
	static std::vector<GameObject*> allGameObjectsINGAME;
	static std::vector<Light*> AllLightINGAME; //PLUS ULTRA
	static std::vector<Weapon*> allWeaponsINGAME;
	
	static void sortAddTexture(unsigned int front, unsigned int back, Texture* ELEM);
	static void sortAddMesh(unsigned int front, unsigned int back, Mesh* ELEM);
	static void sortAddGameObject(unsigned int front, unsigned int back, GameObject* ELEM);
	static void sortAddLight(unsigned int front, unsigned int back, Light* ELEM);
	static void sortAddShader(unsigned int front, unsigned int back, ShaderProgram* ELEM);
	static void sortAddCamera(unsigned int front, unsigned int back, Camera* ELEM);
	static void sortAddEntity(unsigned int front, unsigned int back, Transform* ELEM);
	static void sortAddHitbox(unsigned int front, unsigned int back, Hitbox2D* ELEM);
	static void sortAddFramebuffer(unsigned int front, unsigned int back, Framebuffer* ELEM);
	static void sortAddWeapon(unsigned int front, unsigned int back, Weapon* ELEM);
	static void sortAddMaterial(unsigned int front, unsigned int back, Material* ELEM);

	static void sortAddGameObjectINGAME(unsigned int front, unsigned int back, GameObject* ELEM);
	static void sortAddLightINGAME(unsigned int front, unsigned int back, Light* ELEM);
	static void sortAddCameraINGAME(unsigned int front, unsigned int back, Camera* ELEM);
	static void sortAddEntityINGAME(unsigned int front, unsigned int back, Transform* ELEM);
	static void sortAddWeaponINGAME(unsigned int front, unsigned int back, Weapon* ELEM);
	
	static Texture* findTexture(unsigned int front, unsigned int back, std::string _NAME);
	static Mesh* findMesh(unsigned int front, unsigned int back, std::string _NAME);
	static GameObject* findGameObject(unsigned int front, unsigned int back, std::string _NAME);
	static Light* findLight(unsigned int front, unsigned int back, std::string _NAME);
	static ShaderProgram* findShader(unsigned int front, unsigned int back, std::string _NAME);
	static Transform* findEntity(unsigned int front, unsigned int back, std::string _NAME);
	static Camera* findCamera(unsigned int front, unsigned int back, std::string _NAME);
	static Hitbox2D* findHitbox(unsigned int front, unsigned int back, std::string _NAME);
	static Framebuffer* findFramebuffer(unsigned int front, unsigned int back, std::string _NAME);
	static Weapon* findWeapon(unsigned int front, unsigned int back, std::string _NAME);
	static Material* findMaterial(unsigned int front, unsigned int back, std::string _NAME);

	static int findGameObjectINGAME(unsigned int front, unsigned int back, GameObject* _ADR);
	static int findLightINGAME(unsigned int front, unsigned int back, Light* _ADR);
	static int findEntityINGAME(unsigned int front, unsigned int back, Transform* _ADR);
	static int findCameraINGAME(unsigned int front, unsigned int back, Camera* _ADR);
	static int findWeaponINGAME(unsigned int front, unsigned int back, Weapon* _ADR);

	static Texture* searchForTexture(std::string _NAME);
	static Mesh* searchForMesh(std::string _NAME);
	static GameObject* searchForGameObject(std::string _NAME);
	static Light* searchForLight(std::string _NAME);
	static ShaderProgram* searchForShader(std::string _NAME);
	static Transform* searchForTransform(std::string _NAME);
	static Camera* searchForCamera(std::string _NAME);
	static Hitbox2D* searchForHitbox(std::string _NAME);
	static Framebuffer* searchForFramebuffer(std::string _NAME);
	static Weapon* searchForWeapon(std::string _NAME);
	static Material* searchForMaterial(std::string _NAME);

	static void destroyObjectINGAME(Transform* _OBJ);
	static void destroyChildrenINGAME(Transform* OBJ);

	static Boundary* getBoundary(std::string _NAME);
	static Destructable* getDestructable(std::string _NAME);
	static Player* getPlayer(std::string _NAME);
	static BasePlate* getBasePlate(std::string _NAME);
	static Light* getLight(std::string _NAME);
	static Hitbox2D* getHitbox(std::string _NAME);
	static Mesh* getMesh(std::string _NAME);
	static Texture* getTexture(std::string _NAME);
	static Camera* getCamera(std::string _NAME);
	static Transform* getEntity(std::string _NAME);
	static ShaderProgram* getShader(std::string _NAME);
	static GameObject* getObject(std::string _NAME);
	static Framebuffer* getFramebuffer(std::string _NAME);
	static Weapon* getWeapon(std::string _NAME);
	static Material* getMaterial(std::string _NAME);

	static Boundary* getCloneOfBoundary(std::string _NAME);
	static Destructable* getCloneOfDestructable(std::string _NAME);
	static Player* getCloneOfPlayer(std::string _NAME);
	static BasePlate* getCloneOfBasePlate(std::string _NAME);
	static Light* getCloneOfLight(std::string _NAME);
	static Camera* getCloneOfCamera(std::string _NAME);
	static Transform* getCloneOfEntity(std::string _NAME);
	static GameObject* getCloneOfObject(std::string _NAME);
	static Weapon* getCloneOfWeapon(std::string _NAME);

	static void cloneChildren(Transform* _TF);
};

typedef ResourceManager rm;