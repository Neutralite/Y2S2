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
#include "WEAPON_Mine.h"
#include "WEAPON_Hammer.h"
#include "WEAPON_Axe.h"
#include "Material.h"
#include "Text.h"
#include "Powerup.h"
#include "RecolorObject.h"
#include "FModWrapper.h"

class ResourceManager
{
public:
	static void addEntity(Transform* entity);
	static void addShader(ShaderProgram* shader);
	static void addTexture(Texture* TEX);
	static void addLUT(Texture* LUT);
	static void addMesh(Mesh* MESH);
	static void addHitbox(Hitbox2D* HB);
	static void addFramebuffer(Framebuffer* FB);
	static void addMaterial(Material* _M);
	static void addSound(Sound* _SOUND);

	static void addEntityINGAME(Transform* entity);

	static std::vector<ShaderProgram*> Shaders;
	static std::vector<Transform*> Transforms;
	static std::vector<Camera*> Cameras;
	static std::vector<Material*> Materials;
	static std::vector<Sound*> allSounds;
	static std::vector<Text*> Fonts;

	static std::vector<Texture*> allTextures;
	static std::vector<Texture*> allLUTS;
	static std::vector<Mesh*> allMeshes;
	static std::vector<GameObject*> allGameObjects;
	static std::vector<RecolorObject*> allRecolorObjects;
	static std::vector<Light*> AllLight; //PLUS ULTRA
	static std::vector<Hitbox2D*> allHitboxes;
	static std::vector<Framebuffer*> allFramebuffers;
	static std::vector<Weapon*> allWeapons;
	static std::vector<Powerup*> allPowerups;

	static std::vector<Transform*> TransformsINGAME;
	static std::vector<Camera*> CamerasINGAME;
	static std::vector<GameObject*> allGameObjectsINGAME;
	static std::vector<RecolorObject*> allRecolorObjectsINGAME;
	static std::vector<Light*> AllLightINGAME; //PLUS ULTRA
	static std::vector<Weapon*> allWeaponsINGAME;
	static std::vector<Text*> allTextINGAME;
	static std::vector<Powerup*> allPowerupsINGAME;

	template <class T>
	static void sortAdd(unsigned int front, unsigned int back, T* ELEM, std::vector<T*> *LIST);
	template <class T>
	static void sortAddINGAME(unsigned int front, unsigned int back, T* ELEM, std::vector<T*> *LIST);
	
	template <class T>
	static T* find(unsigned int front, unsigned int back, std::string _NAME, std::vector<T*> *LIST);
	template <class T>
	static int findINGAME(unsigned int front, unsigned int back, T* _ADR, std::vector<T*> *LIST);

	template <class T>
	static int protectedFindINGAME(T* _ADR, std::vector<T*> *LIST);

	static Texture* searchForTexture(std::string _NAME);
	static Texture* searchForLUT(std::string _NAME);
	static Mesh* searchForMesh(std::string _NAME);
	static GameObject* searchForGameObject(std::string _NAME);
	static RecolorObject* searchForRecolorObject(std::string _NAME);
	static Light* searchForLight(std::string _NAME);
	static ShaderProgram* searchForShader(std::string _NAME);
	static Transform* searchForTransform(std::string _NAME);
	static Camera* searchForCamera(std::string _NAME);
	static Hitbox2D* searchForHitbox(std::string _NAME);
	static Framebuffer* searchForFramebuffer(std::string _NAME);
	static Weapon* searchForWeapon(std::string _NAME);
	static Material* searchForMaterial(std::string _NAME);
	static Text* searchForFont(std::string _NAME);
	static Powerup* searchForPowerup(std::string _NAME);
	static Sound* searchForSounds(std::string _NAME);

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
	static Texture* getLUT(std::string _NAME);
	static Camera* getCamera(std::string _NAME);
	static Transform* getEntity(std::string _NAME);
	static ShaderProgram* getShader(std::string _NAME);
	static GameObject* getObject(std::string _NAME);
	static RecolorObject* getRecolorObject(std::string _NAME);
	static Framebuffer* getFramebuffer(std::string _NAME);
	static Weapon* getWeapon(std::string _NAME);
	static Material* getMaterial(std::string _NAME);
	static Sound* getSound(std::string _NAME);
	static Text* getFont(std::string _NAME);
	static Powerup* getPowerup(std::string _NAME);

	static Boundary* getCloneOfBoundary(std::string _NAME);
	static Destructable* getCloneOfDestructable(std::string _NAME);
	static Player* getCloneOfPlayer(std::string _NAME);
	static BasePlate* getCloneOfBasePlate(std::string _NAME);
	static Light* getCloneOfLight(std::string _NAME);
	static Camera* getCloneOfCamera(std::string _NAME);
	static Transform* getCloneOfEntity(std::string _NAME);
	static GameObject* getCloneOfObject(std::string _NAME);
	static RecolorObject* getCloneOfRecolorObject(std::string _NAME);
	static Weapon* getCloneOfWeapon(std::string _NAME);
	static Text* getCloneOfText(std::string _NAME);
	static Powerup* getCloneOfPowerup(std::string _NAME);

	static void cloneChildren(Transform* _TF);

	static void order66();
};

typedef ResourceManager rm;

template<class T>
inline void ResourceManager::sortAdd(unsigned int front, unsigned int back, T * ELEM, std::vector<T*> *LIST)
{
	unsigned int mid = (front + back) / 2;
	if ((*LIST)[mid]->getName() == ELEM->getName())
	{
		LIST->insert(LIST->begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if ((*LIST)[front]->getName() > ELEM->getName())
			LIST->insert(LIST->begin() + front, ELEM);
		else if ((*LIST)[back]->getName() > ELEM->getName())
			LIST->insert(LIST->begin() + back, ELEM);
		else
			LIST->insert(LIST->begin() + back + 1, ELEM);
	}
	else if ((*LIST)[mid]->getName() > ELEM->getName())
	{
		sortAdd(front, mid, ELEM, LIST);
	}
	else
	{
		sortAdd(mid, back, ELEM, LIST);
	}
}

template<class T>
inline void ResourceManager::sortAddINGAME(unsigned int front, unsigned int back, T * ELEM, std::vector<T*> *LIST)
{
	unsigned int mid = (front + back) / 2;
	if ((*LIST)[mid] == ELEM)
	{
		LIST->insert(LIST->begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if ((*LIST)[front] > ELEM)
			LIST->insert(LIST->begin() + front, ELEM);
		else if ((*LIST)[back] > ELEM)
			LIST->insert(LIST->begin() + back, ELEM);
		else
			LIST->insert(LIST->begin() + back + 1, ELEM);
	}
	else if ((*LIST)[mid] > ELEM)
	{
		sortAddINGAME(front, mid, ELEM, LIST);
	}
	else
	{
		sortAddINGAME(mid, back, ELEM, LIST);
	}
}

template<class T>
inline T * ResourceManager::find(unsigned int front, unsigned int back, std::string _NAME, std::vector<T*>* LIST)
{
	unsigned int mid = (front + back) / 2;
	if ((*LIST)[mid]->getName() == _NAME)
	{
		return (*LIST)[mid];
	}
	else if (back - front <= 1)
	{
		if ((*LIST)[front]->getName() == _NAME)
			return (*LIST)[front];
		else if ((*LIST)[back]->getName() == _NAME)
			return (*LIST)[back];
		else
			return nullptr;
	}
	else if ((*LIST)[mid]->getName() > _NAME)
	{
		return find(front, mid, _NAME, LIST);
	}
	else
	{
		return find(mid, back, _NAME, LIST);
	}
}

template<class T>
inline int ResourceManager::findINGAME(unsigned int front, unsigned int back, T* _ADR, std::vector<T*>* LIST)
{
	unsigned int mid = (front + back) / 2;
	if ((*LIST)[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if ((*LIST)[front] == _ADR)
			return front;
		else if ((*LIST)[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if ((*LIST)[mid] > _ADR)
	{
		return findINGAME(front, mid, _ADR, LIST);
	}
	else
	{
		return findINGAME(mid, back, _ADR, LIST);
	}
}

template<class T>
inline int ResourceManager::protectedFindINGAME(T * _ADR, std::vector<T*>* LIST)
{
	if (LIST->size() == 0)
		return -1;
	else
		return findINGAME(0, LIST->size() - 1, _ADR, LIST);
}
