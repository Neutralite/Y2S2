#include "ResourceManager.h"
#include <iostream>

std::vector<Transform*> ResourceManager::Transforms;
std::vector<ShaderProgram*> ResourceManager::Shaders;
std::vector<Camera*> ResourceManager::Cameras;
std::vector<Material*> ResourceManager::Materials;
std::vector<Text*> ResourceManager::Fonts;

std::vector<Texture*> ResourceManager::allTextures;
std::vector<Mesh*> ResourceManager::allMeshes;
std::vector<GameObject*> ResourceManager::allGameObjects;
std::vector<RecolorObject*> ResourceManager::allRecolorObjects;
std::vector<Light*> ResourceManager::AllLight; //PLUS ULTRA
std::vector<Hitbox2D*> ResourceManager::allHitboxes;
std::vector<Framebuffer*> ResourceManager::allFramebuffers;
std::vector<Weapon*> ResourceManager::allWeapons;
std::vector<Powerup*> ResourceManager::allPowerups;


std::vector<Transform*> ResourceManager::TransformsINGAME;
std::vector<Camera*> ResourceManager::CamerasINGAME;
std::vector<GameObject*> ResourceManager::allGameObjectsINGAME;
std::vector<RecolorObject*> ResourceManager::allRecolorObjectsINGAME;
std::vector<Light*> ResourceManager::AllLightINGAME; //PLUS ULTRA
std::vector<Weapon*> ResourceManager::allWeaponsINGAME;
std::vector<Text*> ResourceManager::allTextINGAME;
std::vector<Powerup*> ResourceManager::allPowerupsINGAME;

void ResourceManager::addEntity(Transform * entity)
{
	//Transforms.push_back(entity);
	if (Transforms.size() == 0)
		Transforms.push_back(entity);
	else
		sortAdd(0, Transforms.size() - 1, entity, &Transforms);
	if (Camera* camera = dynamic_cast<Camera*>(entity))
	{
		//Cameras.push_back(camera);
		if (Cameras.size() == 0)
			Cameras.push_back(camera);
		else
			sortAdd(0, Cameras.size() - 1, camera, &Cameras);
	}
	else if (Light* light = dynamic_cast<Light*>(entity))
	{
		//AllLight.push_back(light);
		if (AllLight.size() == 0)
			AllLight.push_back(light);
		else
			sortAdd(0, AllLight.size() - 1, light, &AllLight);
	}
	else if (Weapon* _W = dynamic_cast<Weapon*>(entity))
	{
		//AllLight.push_back(light);
		if (allWeapons.size() == 0)
			allWeapons.push_back(_W);
		else
			sortAdd(0, allWeapons.size() - 1, _W, &allWeapons);
	}
	else if (Powerup* _PUP = dynamic_cast<Powerup*>(entity))
	{
		//AllLight.push_back(light);
		if (allPowerups.size() == 0)
			allPowerups.push_back(_PUP);
		else
			sortAdd(0, allPowerups.size() - 1, _PUP, &allPowerups);
	}
	else if (RecolorObject* _RECOLOR = dynamic_cast<RecolorObject*>(entity))
	{
		//AllLight.push_back(light);
		if (allRecolorObjects.size() == 0)
			allRecolorObjects.push_back(_RECOLOR);
		else
			sortAdd(0, allRecolorObjects.size() - 1, _RECOLOR, &allRecolorObjects);
	}
	else if (GameObject* _GO = dynamic_cast<GameObject*>(entity))
	{
		//allGameObjects.push_back(_GO);
		if (allGameObjects.size() == 0)
			allGameObjects.push_back(_GO);
		else
			sortAdd(0, allGameObjects.size() - 1, _GO, &allGameObjects);
	}
	else if (Text* _T = dynamic_cast<Text*>(entity))
	{
		if (Fonts.size() == 0)
			Fonts.push_back(_T);
		else
			sortAdd(0, Fonts.size() - 1, _T, &Fonts);
	}
	else
	{

	}
}

void ResourceManager::addShader(ShaderProgram * shader)
{
	//Shaders.push_back(shader);
	if (Shaders.size() == 0)
		Shaders.push_back(shader);
	else
		sortAdd(0, Shaders.size() - 1, shader, &Shaders);
}

void ResourceManager::addTexture(Texture * TEX)
{
	//allTextures.push_back(TEX);
	if (allTextures.size() == 0)
		allTextures.push_back(TEX);
	else
		sortAdd(0, allTextures.size() - 1, TEX, &allTextures);
}

void ResourceManager::addMesh(Mesh * MESH)
{
	//allMeshes.push_back(MESH);
	//SAT_DEBUG_LOG(MESH->getName().c_str());
	if (allMeshes.size() == 0)
		allMeshes.push_back(MESH);
	else
		sortAdd(0, allMeshes.size() - 1, MESH, &allMeshes);
}

void ResourceManager::addHitbox(Hitbox2D * HB)
{
	//SAT_DEBUG_LOG(HB->getName().c_str());
	if (allHitboxes.size() == 0)
		allHitboxes.push_back(HB);
	else
		sortAdd(0, allHitboxes.size() - 1, HB, &allHitboxes);
}

void ResourceManager::addFramebuffer(Framebuffer * FB)
{
	if (allFramebuffers.size() == 0)
		allFramebuffers.push_back(FB);
	else
		sortAdd(0, allFramebuffers.size() - 1, FB, &allFramebuffers);
}

void ResourceManager::addMaterial(Material * _M)
{

	if (Materials.size() == 0)
		Materials.push_back(_M);
	else
		sortAdd(0, Materials.size() - 1, _M, &Materials);
}

void ResourceManager::addEntityINGAME(Transform * entity)
{
	if (TransformsINGAME.size() == 0)
		TransformsINGAME.push_back(entity);
	else
		sortAddINGAME(0, TransformsINGAME.size() - 1, entity, &TransformsINGAME);
	if (Camera* camera = dynamic_cast<Camera*>(entity))
	{
		if (CamerasINGAME.size() == 0)
			CamerasINGAME.push_back(camera);
		else
			sortAddINGAME(0, CamerasINGAME.size() - 1, camera, &CamerasINGAME);
	}
	else if (Light* light = dynamic_cast<Light*>(entity))
	{
		if (AllLightINGAME.size() == 0)
			AllLightINGAME.push_back(light);
		else
			sortAddINGAME(0, AllLightINGAME.size() - 1, light, &AllLightINGAME);
	}
	else if (Weapon* W = dynamic_cast<Weapon*>(entity))
	{
		if (allWeaponsINGAME.size() == 0)
			allWeaponsINGAME.push_back(W);
		else
			sortAddINGAME(0, allWeaponsINGAME.size() - 1, W, &allWeaponsINGAME);
	}
	else if (Powerup* _PUP = dynamic_cast<Powerup*>(entity))
	{
		if (allPowerupsINGAME.size() == 0)
			allPowerupsINGAME.push_back(_PUP);
		else
			sortAddINGAME(0, allPowerupsINGAME.size() - 1, _PUP, &allPowerupsINGAME);
	}
	else if (RecolorObject* _RECOLOR = dynamic_cast<RecolorObject*>(entity))
	{
		if (allRecolorObjectsINGAME.size() == 0)
			allRecolorObjectsINGAME.push_back(_RECOLOR);
		else
			sortAddINGAME(0, allRecolorObjectsINGAME.size() - 1, _RECOLOR, &allRecolorObjectsINGAME);
	}
	else if (GameObject* _GO = dynamic_cast<GameObject*>(entity))
	{
		if (allGameObjectsINGAME.size() == 0)
			allGameObjectsINGAME.push_back(_GO);
		else
			sortAddINGAME(0, allGameObjectsINGAME.size() - 1, _GO, &allGameObjectsINGAME);
	}
	else if (Text* _T = dynamic_cast<Text*>(entity))
	{
		if (allTextINGAME.size() == 0)
			allTextINGAME.push_back(_T);
		else
			sortAddINGAME(0, allTextINGAME.size() - 1, _T, &allTextINGAME);
	}
	else
	{

	}
}

Texture * ResourceManager::searchForTexture(std::string _NAME)
{
	if (allTextures.size() == 0)
		return nullptr;
	else
		return find(0, allTextures.size() - 1, _NAME, &allTextures);
}

Mesh * ResourceManager::searchForMesh(std::string _NAME)
{
	if (allMeshes.size() == 0)
		return nullptr;
	else
		return find(0, allMeshes.size() - 1, _NAME, &allMeshes);
}

GameObject * ResourceManager::searchForGameObject(std::string _NAME)
{
	if (allGameObjects.size() == 0)
		return nullptr;
	else
		return find(0, allGameObjects.size() - 1, _NAME, &allGameObjects);
}

RecolorObject * ResourceManager::searchForRecolorObject(std::string _NAME)
{
	if (allRecolorObjects.size() == 0)
		return nullptr;
	else
		return find(0, allRecolorObjects.size() - 1, _NAME, &allRecolorObjects);
}

Light * ResourceManager::searchForLight(std::string _NAME)
{
	if (AllLight.size() == 0)
		return nullptr;
	else
		return find(0, AllLight.size() - 1, _NAME, &AllLight);
}

ShaderProgram * ResourceManager::searchForShader(std::string _NAME)
{
	if (Shaders.size() == 0)
		return nullptr;
	else
		return find(0, Shaders.size() - 1, _NAME, &Shaders);
}

Transform * ResourceManager::searchForTransform(std::string _NAME)
{
	if (Transforms.size() == 0)
		return nullptr;
	else
		return find(0, Transforms.size() - 1, _NAME, &Transforms);
}

Camera * ResourceManager::searchForCamera(std::string _NAME)
{
	if (Cameras.size() == 0)
		return nullptr;
	else
		return find(0, Cameras.size() - 1, _NAME, &Cameras);
}

Hitbox2D * ResourceManager::searchForHitbox(std::string _NAME)
{
	if (allHitboxes.size() == 0)
		return nullptr;
	else
		return find(0, allHitboxes.size() - 1, _NAME, &allHitboxes);
}

Framebuffer * ResourceManager::searchForFramebuffer(std::string _NAME)
{
	if (allFramebuffers.size() == 0)
		return nullptr;
	else
		return find(0, allFramebuffers.size() - 1, _NAME, &allFramebuffers);
}

Weapon * ResourceManager::searchForWeapon(std::string _NAME)
{
	if (allWeapons.size() == 0)
		return nullptr;
	else
		return find(0, allWeapons.size() - 1, _NAME, &allWeapons);
}

Material * ResourceManager::searchForMaterial(std::string _NAME)
{
	if (Materials.size() == 0)
		return nullptr;
	else
		return find(0, Materials.size() - 1, _NAME, &Materials);
}

Text * ResourceManager::searchForFont(std::string _NAME)
{
	if (Fonts.size() == 0)
		return nullptr;
	else
		return find(0, Fonts.size() - 1, _NAME, &Fonts);
}

Powerup * ResourceManager::searchForPowerup(std::string _NAME)
{
	if (allPowerups.size() == 0)
		return nullptr;
	else
		return find(0, allPowerups.size() - 1, _NAME, &allPowerups);
}

void ResourceManager::destroyObjectINGAME(Transform * _OBJ)
{
	//destroyChildrenINGAME(_OBJ);
	if (_OBJ)
	{

		int iter = protectedFindINGAME(_OBJ, &TransformsINGAME);
		//std::cout << iter << std::endl;
		if (iter >= 0)
		{
			TransformsINGAME.erase(TransformsINGAME.begin() + iter);
		}

		if (Camera* camera = dynamic_cast<Camera*>(_OBJ))
		{
			iter = protectedFindINGAME(camera, &CamerasINGAME);
			if (iter >= 0)
			{
				CamerasINGAME.erase(CamerasINGAME.begin() + iter);
			}
		}

		if (Light* light = dynamic_cast<Light*>(_OBJ))
		{
			iter = protectedFindINGAME(light, &AllLightINGAME);
			if (iter >= 0)
			{
				AllLightINGAME.erase(AllLightINGAME.begin() + iter);
			}
		}

		if (Weapon* _W = dynamic_cast<Weapon*>(_OBJ))
		{
			iter = protectedFindINGAME(_W, &allWeaponsINGAME);
			if (iter >= 0)
			{
				allWeaponsINGAME.erase(allWeaponsINGAME.begin() + iter);
			}
		}

		if (Powerup* _PUP = dynamic_cast<Powerup*>(_OBJ))
		{
			iter = protectedFindINGAME(_PUP, &allPowerupsINGAME);
			if (iter >= 0)
			{
				allPowerupsINGAME.erase(allPowerupsINGAME.begin() + iter);
			}
		}

		if (RecolorObject* RECOLOR = dynamic_cast<RecolorObject*>(_OBJ))
		{
			iter = protectedFindINGAME(RECOLOR, &allRecolorObjectsINGAME);
			if (iter >= 0)
			{
				allRecolorObjectsINGAME.erase(allRecolorObjectsINGAME.begin() + iter);
			}
		}

		if (GameObject* _GO = dynamic_cast<GameObject*>(_OBJ))
		{
			iter = protectedFindINGAME(_GO, &allGameObjectsINGAME);
			if (iter >= 0)
			{
				allGameObjectsINGAME.erase(allGameObjectsINGAME.begin() + iter);
			}
		}

		if (Text* _T = dynamic_cast<Text*>(_OBJ))
		{
			iter = protectedFindINGAME(_T, &allTextINGAME);
			if (iter >= 0)
			{
				allTextINGAME.erase(allTextINGAME.begin() + iter);
			}
		}

		delete _OBJ;
	}
	else
	{
		std::cout << "NULL" << std::endl;
	}
}

void ResourceManager::destroyChildrenINGAME(Transform * OBJ)
{
	int CHILDREN = OBJ->getChildren().size();
	for (int i = 0; i < CHILDREN; i++)
	{
		destroyObjectINGAME(OBJ->getChildren().at(i));
	}
}

Boundary * ResourceManager::getBoundary(std::string _NAME)
{
	GameObject* passing = ResourceManager::searchForGameObject(_NAME);
	if (Boundary* SUB = dynamic_cast<Boundary*>(passing))
	{
		return SUB;
	}
	else
	{
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
		return nullptr;
	}
}

Destructable * ResourceManager::getDestructable(std::string _NAME)
{
	GameObject* passing = ResourceManager::searchForGameObject(_NAME);
	if (Destructable* SUB = dynamic_cast<Destructable*>(passing))
	{
		return SUB;
	}
	else
	{
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
		return nullptr;
	}
}

Player * ResourceManager::getPlayer(std::string _NAME)
{
	GameObject* passing = ResourceManager::searchForGameObject(_NAME);
	if (Player* SUB = dynamic_cast<Player*>(passing))
	{
		return SUB;
	}
	else
	{
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
		return nullptr;
	}
}

BasePlate * ResourceManager::getBasePlate(std::string _NAME)
{
	GameObject* passing = ResourceManager::searchForGameObject(_NAME);
	if (BasePlate* SUB = dynamic_cast<BasePlate*>(passing))
	{
		return SUB;
	}
	else
	{
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
		return nullptr;
	}
}

Light * ResourceManager::getLight(std::string _NAME)
{
	Light* SUB = ResourceManager::searchForLight(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Hitbox2D * ResourceManager::getHitbox(std::string _NAME)
{
	Hitbox2D* SUB = ResourceManager::searchForHitbox(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Mesh * ResourceManager::getMesh(std::string _NAME)
{
	Mesh* SUB = ResourceManager::searchForMesh(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Texture * ResourceManager::getTexture(std::string _NAME)
{
	Texture* SUB = ResourceManager::searchForTexture(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Camera * ResourceManager::getCamera(std::string _NAME)
{
	Camera* SUB = ResourceManager::searchForCamera(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Transform * ResourceManager::getEntity(std::string _NAME)
{
	Transform* SUB = ResourceManager::searchForTransform(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

ShaderProgram * ResourceManager::getShader(std::string _NAME)
{
	ShaderProgram* SUB = ResourceManager::searchForShader(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

GameObject * ResourceManager::getObject(std::string _NAME)
{
	GameObject* SUB = ResourceManager::searchForGameObject(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

RecolorObject * ResourceManager::getRecolorObject(std::string _NAME)
{
	RecolorObject* SUB = ResourceManager::searchForRecolorObject(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Framebuffer * ResourceManager::getFramebuffer(std::string _NAME)
{
	Framebuffer* SUB = ResourceManager::searchForFramebuffer(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Weapon * ResourceManager::getWeapon(std::string _NAME)
{
	Weapon* SUB = ResourceManager::searchForWeapon(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Material * ResourceManager::getMaterial(std::string _NAME)
{
	Material* SUB = ResourceManager::searchForMaterial(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Text * ResourceManager::getFont(std::string _NAME)
{
	Text* SUB = ResourceManager::searchForFont(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Powerup * ResourceManager::getPowerup(std::string _NAME)
{
	Powerup* SUB = ResourceManager::searchForPowerup(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Boundary * ResourceManager::getCloneOfBoundary(std::string _NAME)
{
	Boundary* SUB = getBoundary(_NAME);
	if (SUB)
	{
		Boundary* SUB2 = new Boundary;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Destructable * ResourceManager::getCloneOfDestructable(std::string _NAME)
{
	Destructable* SUB = getDestructable(_NAME);
	if (SUB)
	{
		Destructable* SUB2 = new Destructable;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Player * ResourceManager::getCloneOfPlayer(std::string _NAME)
{
	Player* SUB = getPlayer(_NAME);
	if (SUB)
	{
		Player* SUB2 = new Player;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

BasePlate * ResourceManager::getCloneOfBasePlate(std::string _NAME)
{
	BasePlate* SUB = getBasePlate(_NAME);
	if (SUB)
	{
		BasePlate* SUB2 = new BasePlate;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Light * ResourceManager::getCloneOfLight(std::string _NAME)
{
	Light* SUB = getLight(_NAME);
	if (SUB)
	{
		Light* SUB2 = new Light;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Camera * ResourceManager::getCloneOfCamera(std::string _NAME)
{
	//std::cout << "T1" << std::endl;
	Camera* SUB = getCamera(_NAME);
	if (SUB)
	{
		//std::cout << "T2" << std::endl;
		Camera* SUB2 = new Camera;
		//std::cout << "T2.5" << std::endl;
		*SUB2 = *SUB;

		//std::cout << "T3" << std::endl;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);

		//std::cout << "T4" << std::endl;

		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Transform * ResourceManager::getCloneOfEntity(std::string _NAME)
{
	Transform* SUB = getEntity(_NAME);
	if (SUB)
	{
		Transform* SUB2 = new Transform;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

GameObject * ResourceManager::getCloneOfObject(std::string _NAME)
{
	GameObject* SUB = getObject(_NAME);
	if (SUB)
	{
		GameObject* SUB2 = new GameObject;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

RecolorObject * ResourceManager::getCloneOfRecolorObject(std::string _NAME)
{
	RecolorObject* SUB = getRecolorObject(_NAME);
	if (SUB)
	{
		RecolorObject* SUB2 = new RecolorObject;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Weapon * ResourceManager::getCloneOfWeapon(std::string _NAME)
{
	Weapon* SUB = getWeapon(_NAME);
	if (SUB)
	{
		Weapon* SUB2 = nullptr;
		if (SUB->TT == Transform::TransformType::TYPE_Mine)
			SUB2 = new Mine;
		else if (SUB->TT == Transform::TransformType::TYPE_Hammer)
			SUB2 = new Hammer;
		else if (SUB->TT == Transform::TransformType::TYPE_Axe)
			SUB2 = new Axe;
		*SUB2 = *SUB;

		if (SUB2)
		{
			ResourceManager::addEntityINGAME(SUB2);
			cloneChildren(SUB2);
		}
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Text * ResourceManager::getCloneOfText(std::string _NAME)
{
	Text* SUB = getFont(_NAME);
	if (SUB)
	{
		Text* SUB2 = new Text;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

Powerup * ResourceManager::getCloneOfPowerup(std::string _NAME)
{
	Powerup* SUB = getPowerup(_NAME);
	if (SUB)
	{
		Powerup* SUB2 = new Powerup;
		*SUB2 = *SUB;

		ResourceManager::addEntityINGAME(SUB2);
		cloneChildren(SUB2);
		return SUB2;
	}
	else
	{
		return nullptr;
	}
}

void ResourceManager::cloneChildren(Transform * _TF)
{
	unsigned int TFsize = _TF->getChildren().size();
	for (unsigned int i = 0; i < TFsize; ++i)
	{
		Transform* _TF2 = _TF->getChildren().at(0);
		_TF->removeChild(_TF->getChildren().at(0));
		if (Boundary* SUB = dynamic_cast<Boundary*>(_TF2))
		{
			SUB = getCloneOfBoundary(_TF2->getName());
			_TF->addChild(SUB);
		}
		else if (BasePlate* SUB1 = dynamic_cast<BasePlate*>(_TF2))
		{
			SUB1 = getCloneOfBasePlate(_TF2->getName());
			_TF->addChild(SUB1);
		}
		else if (Destructable* SUB2 = dynamic_cast<Destructable*>(_TF2))
		{
			SUB2 = getCloneOfDestructable(_TF2->getName());
			_TF->addChild(SUB2);
		}
		else if (Player* SUB3 = dynamic_cast<Player*>(_TF2))
		{
			SUB3 = getCloneOfPlayer(_TF2->getName());
			_TF->addChild(SUB3);
		}
		else if (Weapon* SUB7 = dynamic_cast<Weapon*>(_TF2))
		{
			SUB7 = getCloneOfWeapon(_TF2->getName());
			_TF->addChild(SUB7);
		}
		else if (Powerup* SUB9 = dynamic_cast<Powerup*>(_TF2))
		{
			SUB9 = getCloneOfPowerup(_TF2->getName());
			_TF->addChild(SUB9);
		}
		else if (RecolorObject* SUB10 = dynamic_cast<RecolorObject*>(_TF2))
		{
			SUB10 = getCloneOfRecolorObject(_TF2->getName());
			_TF->addChild(SUB10);
		}
		else if (GameObject* SUB4 = dynamic_cast<GameObject*>(_TF2))
		{
			SUB4 = getCloneOfObject(_TF2->getName());
			_TF->addChild(SUB4);
		}
		else if (Light* SUB5 = dynamic_cast<Light*>(_TF2))
		{
			SUB5 = getCloneOfLight(_TF2->getName());
			_TF->addChild(SUB5);
		}
		else if (Camera* SUB6 = dynamic_cast<Camera*>(_TF2))
		{
			SUB6 = getCloneOfCamera(_TF2->getName());
			_TF->addChild(SUB6);
		}
		else if (Text* SUB8 = dynamic_cast<Text*>(_TF2))
		{
			SUB8 = getCloneOfText(_TF2->getName());
			_TF->addChild(SUB8);
		}
		else
		{
			_TF2 = getCloneOfEntity(_TF2->getName());
			_TF->addChild(_TF2);
		}
	}
}

void ResourceManager::order66()
{
	while (TransformsINGAME.size() > 0)
	{
		//std::cout << TransformsINGAME.size() << std::endl;
		//std::cout << TransformsINGAME[TransformsINGAME.size() - 1]->getName() << std::endl;
		destroyObjectINGAME(TransformsINGAME[(TransformsINGAME.size() - 1) / 2]);
	}
}

