#include "ResourceManager.h"
#include <iostream>

std::vector<Transform*> ResourceManager::Transforms;
std::vector<ShaderProgram*> ResourceManager::Shaders;
std::vector<Camera*> ResourceManager::Cameras;

std::vector<Texture*> ResourceManager::allTextures;
std::vector<Mesh*> ResourceManager::allMeshes;
std::vector<GameObject*> ResourceManager::allGameObjects;
std::vector<Light*> ResourceManager::AllLight; //PLUS ULTRA
std::vector<Hitbox2D*> ResourceManager::allHitboxes;
std::vector<Framebuffer*> ResourceManager::allFramebuffers;
std::vector<Weapon*> ResourceManager::allWeapons;


std::vector<Transform*> ResourceManager::TransformsINGAME;
std::vector<Camera*> ResourceManager::CamerasINGAME;
std::vector<GameObject*> ResourceManager::allGameObjectsINGAME;
std::vector<Light*> ResourceManager::AllLightINGAME; //PLUS ULTRA
std::vector<Weapon*> ResourceManager::allWeaponsINGAME;

void ResourceManager::addEntity(Transform * entity)
{
	//Transforms.push_back(entity);
	if (Transforms.size() == 0)
		Transforms.push_back(entity);
	else
		sortAddEntity(0, Transforms.size() - 1, entity);
	if (Camera* camera = dynamic_cast<Camera*>(entity))
	{
		//Cameras.push_back(camera);
		if (Cameras.size() == 0)
			Cameras.push_back(camera);
		else
			sortAddCamera(0, Cameras.size() - 1, camera);
	}
	else if (Light* light = dynamic_cast<Light*>(entity))
	{
		//AllLight.push_back(light);
		if (AllLight.size() == 0)
			AllLight.push_back(light);
		else
			sortAddLight(0, AllLight.size() - 1, light);
	}
	else if (Weapon* _W = dynamic_cast<Weapon*>(entity))
	{
		//AllLight.push_back(light);
		if (allWeapons.size() == 0)
			allWeapons.push_back(_W);
		else
			sortAddWeapon(0, allWeapons.size() - 1, _W);
	}
	else if (GameObject* _GO = dynamic_cast<GameObject*>(entity))
	{
		//allGameObjects.push_back(_GO);
		if (allGameObjects.size() == 0)
			allGameObjects.push_back(_GO);
		else
			sortAddGameObject(0, allGameObjects.size() - 1, _GO);
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
		sortAddShader(0, Shaders.size() - 1, shader);
}

void ResourceManager::addTexture(Texture * TEX)
{
	//allTextures.push_back(TEX);
	if (allTextures.size() == 0)
		allTextures.push_back(TEX);
	else
		sortAddTexture(0, allTextures.size() - 1, TEX);
}

void ResourceManager::addMesh(Mesh * MESH)
{
	//allMeshes.push_back(MESH);
	//SAT_DEBUG_LOG(MESH->getName().c_str());
	if (allMeshes.size() == 0)
		allMeshes.push_back(MESH);
	else
		sortAddMesh(0, allMeshes.size() - 1, MESH);
}

void ResourceManager::addHitbox(Hitbox2D * HB)
{
	//SAT_DEBUG_LOG(HB->getName().c_str());
	if (allHitboxes.size() == 0)
		allHitboxes.push_back(HB);
	else
		sortAddHitbox(0, allHitboxes.size() - 1, HB);
}

void ResourceManager::addFramebuffer(Framebuffer * FB)
{
	if (allFramebuffers.size() == 0)
		allFramebuffers.push_back(FB);
	else
		sortAddFramebuffer(0, allFramebuffers.size() - 1, FB);
}

void ResourceManager::addEntityINGAME(Transform * entity)
{
	if (TransformsINGAME.size() == 0)
		TransformsINGAME.push_back(entity);
	else
		sortAddEntityINGAME(0, TransformsINGAME.size() - 1, entity);
	if (Camera* camera = dynamic_cast<Camera*>(entity))
	{
		if (CamerasINGAME.size() == 0)
			CamerasINGAME.push_back(camera);
		else
			sortAddCameraINGAME(0, CamerasINGAME.size() - 1, camera);
	}
	else if (Light* light = dynamic_cast<Light*>(entity))
	{
		if (AllLightINGAME.size() == 0)
			AllLightINGAME.push_back(light);
		else
			sortAddLightINGAME(0, AllLightINGAME.size() - 1, light);
	}
	else if (Weapon* W = dynamic_cast<Weapon*>(entity))
	{
		if (allWeaponsINGAME.size() == 0)
			allWeaponsINGAME.push_back(W);
		else
			sortAddWeaponINGAME(0, allWeaponsINGAME.size() - 1, W);
	}
	else if (GameObject* _GO = dynamic_cast<GameObject*>(entity))
	{
		if (allGameObjectsINGAME.size() == 0)
			allGameObjectsINGAME.push_back(_GO);
		else
			sortAddGameObjectINGAME(0, allGameObjectsINGAME.size() - 1, _GO);
	}
	else
	{

	}
}

void ResourceManager::sortAddTexture(unsigned int front, unsigned int back, Texture * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allTextures[mid]->getName() == ELEM->getName())
	{
		allTextures.insert(allTextures.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allTextures[front]->getName() > ELEM->getName())
			allTextures.insert(allTextures.begin() + front, ELEM);
		else if (allTextures[back]->getName() > ELEM->getName())
			allTextures.insert(allTextures.begin() + back, ELEM);
		else
			allTextures.insert(allTextures.begin() + back + 1, ELEM);
	}
	else if (allTextures[mid]->getName() > ELEM->getName())
	{
		sortAddTexture(front, mid, ELEM);
	}
	else
	{
		sortAddTexture(mid, back, ELEM);
	}
}

void ResourceManager::sortAddMesh(unsigned int front, unsigned int back, Mesh * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allMeshes[mid]->getName() == ELEM->getName())
	{
		allMeshes.insert(allMeshes.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allMeshes[front]->getName() > ELEM->getName())
			allMeshes.insert(allMeshes.begin() + front, ELEM);
		else if (allMeshes[back]->getName() > ELEM->getName())
			allMeshes.insert(allMeshes.begin() + back, ELEM);
		else
			allMeshes.insert(allMeshes.begin() + back + 1, ELEM);
	}
	else if (allMeshes[mid]->getName() > ELEM->getName())
	{
		sortAddMesh(front, mid, ELEM);
	}
	else
	{
		sortAddMesh(mid, back, ELEM);
	}
}

void ResourceManager::sortAddGameObject(unsigned int front, unsigned int back, GameObject * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allGameObjects[mid]->getName() == ELEM->getName())
	{
		allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allGameObjects[front]->getName() > ELEM->getName())
			allGameObjects.insert(allGameObjects.begin() + front, ELEM);
		else if (allGameObjects[back]->getName() > ELEM->getName())
			allGameObjects.insert(allGameObjects.begin() + back, ELEM);
		else
			allGameObjects.insert(allGameObjects.begin() + back + 1, ELEM);
	}
	else if (allGameObjects[mid]->getName() > ELEM->getName())
	{
		sortAddGameObject(front, mid, ELEM);
	}
	else
	{
		sortAddGameObject(mid, back, ELEM);
	}
}

void ResourceManager::sortAddLight(unsigned int front, unsigned int back, Light * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (AllLight[mid]->getName() == ELEM->getName())
	{
		AllLight.insert(AllLight.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (AllLight[front]->getName() > ELEM->getName())
			AllLight.insert(AllLight.begin() + front, ELEM);
		else if (AllLight[back]->getName() > ELEM->getName())
			AllLight.insert(AllLight.begin() + back, ELEM);
		else
			AllLight.insert(AllLight.begin() + back + 1, ELEM);
	}
	else if (AllLight[mid]->getName() > ELEM->getName())
	{
		sortAddLight(front, mid, ELEM);
	}
	else
	{
		sortAddLight(mid, back, ELEM);
	}
}

void ResourceManager::sortAddShader(unsigned int front, unsigned int back, ShaderProgram * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (Shaders[mid]->getName() == ELEM->getName())
	{
		Shaders.insert(Shaders.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (Shaders[front]->getName() > ELEM->getName())
			Shaders.insert(Shaders.begin() + front, ELEM);
		else if (Shaders[back]->getName() > ELEM->getName())
			Shaders.insert(Shaders.begin() + back, ELEM);
		else
			Shaders.insert(Shaders.begin() + back + 1, ELEM);
	}
	else if (Shaders[mid]->getName() > ELEM->getName())
	{
		sortAddShader(front, mid, ELEM);
	}
	else
	{
		sortAddShader(mid, back, ELEM);
	}
}

void ResourceManager::sortAddCamera(unsigned int front, unsigned int back, Camera * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (Cameras[mid]->getName() == ELEM->getName())
	{
		Cameras.insert(Cameras.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (Cameras[front]->getName() > ELEM->getName())
			Cameras.insert(Cameras.begin() + front, ELEM);
		else if (Cameras[back]->getName() > ELEM->getName())
			Cameras.insert(Cameras.begin() + back, ELEM);
		else
			Cameras.insert(Cameras.begin() + back + 1, ELEM);
	}
	else if (Cameras[mid]->getName() > ELEM->getName())
	{
		sortAddCamera(front, mid, ELEM);
	}
	else
	{
		sortAddCamera(mid, back, ELEM);
	}
}

void ResourceManager::sortAddEntity(unsigned int front, unsigned int back, Transform * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (Transforms[mid]->getName() == ELEM->getName())
	{
		Transforms.insert(Transforms.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (Transforms[front]->getName() > ELEM->getName())
			Transforms.insert(Transforms.begin() + front, ELEM);
		else if (Transforms[back]->getName() > ELEM->getName())
			Transforms.insert(Transforms.begin() + back, ELEM);
		else
			Transforms.insert(Transforms.begin() + back + 1, ELEM);
	}
	else if (Transforms[mid]->getName() > ELEM->getName())
	{
		sortAddEntity(front, mid, ELEM);
	}
	else
	{
		sortAddEntity(mid, back, ELEM);
	}
}

void ResourceManager::sortAddHitbox(unsigned int front, unsigned int back, Hitbox2D * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allHitboxes[mid]->getName() == ELEM->getName())
	{
		allHitboxes.insert(allHitboxes.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allHitboxes[front]->getName() > ELEM->getName())
			allHitboxes.insert(allHitboxes.begin() + front, ELEM);
		else if (allHitboxes[back]->getName() > ELEM->getName())
			allHitboxes.insert(allHitboxes.begin() + back, ELEM);
		else
			allHitboxes.insert(allHitboxes.begin() + back + 1, ELEM);
	}
	else if (allHitboxes[mid]->getName() > ELEM->getName())
	{
		sortAddHitbox(front, mid, ELEM);
	}
	else
	{
		sortAddHitbox(mid, back, ELEM);
	}
}

void ResourceManager::sortAddFramebuffer(unsigned int front, unsigned int back, Framebuffer * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allFramebuffers[mid]->getName() == ELEM->getName())
	{
		allFramebuffers.insert(allFramebuffers.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allFramebuffers[front]->getName() > ELEM->getName())
			allFramebuffers.insert(allFramebuffers.begin() + front, ELEM);
		else if (allFramebuffers[back]->getName() > ELEM->getName())
			allFramebuffers.insert(allFramebuffers.begin() + back, ELEM);
		else
			allFramebuffers.insert(allFramebuffers.begin() + back + 1, ELEM);
	}
	else if (allFramebuffers[mid]->getName() > ELEM->getName())
	{
		sortAddFramebuffer(front, mid, ELEM);
	}
	else
	{
		sortAddFramebuffer(mid, back, ELEM);
	}
}

void ResourceManager::sortAddWeapon(unsigned int front, unsigned int back, Weapon * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allWeapons[mid]->getName() == ELEM->getName())
	{
		allWeapons.insert(allWeapons.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allWeapons[front]->getName() > ELEM->getName())
			allWeapons.insert(allWeapons.begin() + front, ELEM);
		else if (allWeapons[back]->getName() > ELEM->getName())
			allWeapons.insert(allWeapons.begin() + back, ELEM);
		else
			allWeapons.insert(allWeapons.begin() + back + 1, ELEM);
	}
	else if (allWeapons[mid]->getName() > ELEM->getName())
	{
		sortAddWeapon(front, mid, ELEM);
	}
	else
	{
		sortAddWeapon(mid, back, ELEM);
	}
}

void ResourceManager::sortAddGameObjectINGAME(unsigned int front, unsigned int back, GameObject * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allGameObjectsINGAME[mid] == ELEM)
	{
		allGameObjectsINGAME.insert(allGameObjectsINGAME.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allGameObjectsINGAME[front] > ELEM)
			allGameObjectsINGAME.insert(allGameObjectsINGAME.begin() + front, ELEM);
		else if (allGameObjectsINGAME[back] > ELEM)
			allGameObjectsINGAME.insert(allGameObjectsINGAME.begin() + back, ELEM);
		else
			allGameObjectsINGAME.insert(allGameObjectsINGAME.begin() + back + 1, ELEM);
	}
	else if (allGameObjectsINGAME[mid] > ELEM)
	{
		sortAddGameObjectINGAME(front, mid, ELEM);
	}
	else
	{
		sortAddGameObjectINGAME(mid, back, ELEM);
	}
}

void ResourceManager::sortAddLightINGAME(unsigned int front, unsigned int back, Light * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (AllLightINGAME[mid] == ELEM)
	{
		AllLightINGAME.insert(AllLightINGAME.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (AllLightINGAME[front] > ELEM)
			AllLightINGAME.insert(AllLightINGAME.begin() + front, ELEM);
		else if (AllLightINGAME[back] > ELEM)
			AllLightINGAME.insert(AllLightINGAME.begin() + back, ELEM);
		else
			AllLightINGAME.insert(AllLightINGAME.begin() + back + 1, ELEM);
	}
	else if (AllLightINGAME[mid] > ELEM)
	{
		sortAddLightINGAME(front, mid, ELEM);
	}
	else
	{
		sortAddLightINGAME(mid, back, ELEM);
	}
}

void ResourceManager::sortAddCameraINGAME(unsigned int front, unsigned int back, Camera * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (CamerasINGAME[mid] == ELEM)
	{
		CamerasINGAME.insert(CamerasINGAME.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (CamerasINGAME[front] > ELEM)
			CamerasINGAME.insert(CamerasINGAME.begin() + front, ELEM);
		else if (CamerasINGAME[back] > ELEM)
			CamerasINGAME.insert(CamerasINGAME.begin() + back, ELEM);
		else
			CamerasINGAME.insert(CamerasINGAME.begin() + back + 1, ELEM);
	}
	else if (CamerasINGAME[mid] > ELEM)
	{
		sortAddCameraINGAME(front, mid, ELEM);
	}
	else
	{
		sortAddCameraINGAME(mid, back, ELEM);
	}
}

void ResourceManager::sortAddEntityINGAME(unsigned int front, unsigned int back, Transform * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (TransformsINGAME[mid] == ELEM)
	{
		TransformsINGAME.insert(TransformsINGAME.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (TransformsINGAME[front] > ELEM)
			TransformsINGAME.insert(TransformsINGAME.begin() + front, ELEM);
		else if (TransformsINGAME[back] > ELEM)
			TransformsINGAME.insert(TransformsINGAME.begin() + back, ELEM);
		else
			TransformsINGAME.insert(TransformsINGAME.begin() + back + 1, ELEM);
	}
	else if (TransformsINGAME[mid] > ELEM)
	{
		sortAddEntityINGAME(front, mid, ELEM);
	}
	else
	{
		sortAddEntityINGAME(mid, back, ELEM);
	}
}

void ResourceManager::sortAddWeaponINGAME(unsigned int front, unsigned int back, Weapon * ELEM)
{
	unsigned int mid = (front + back) / 2;
	if (allWeaponsINGAME[mid] == ELEM)
	{
		allWeaponsINGAME.insert(allWeaponsINGAME.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (allWeaponsINGAME[front] > ELEM)
			allWeaponsINGAME.insert(allWeaponsINGAME.begin() + front, ELEM);
		else if (allWeaponsINGAME[back] > ELEM)
			allWeaponsINGAME.insert(allWeaponsINGAME.begin() + back, ELEM);
		else
			allWeaponsINGAME.insert(allWeaponsINGAME.begin() + back + 1, ELEM);
	}
	else if (allWeaponsINGAME[mid] > ELEM)
	{
		sortAddWeaponINGAME(front, mid, ELEM);
	}
	else
	{
		sortAddWeaponINGAME(mid, back, ELEM);
	}
}

Texture * ResourceManager::findTexture(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allTextures[mid]->getName() == _NAME)
	{
		return allTextures[mid];
	}
	else if (back - front <= 1)
	{
		if (allTextures[front]->getName() == _NAME)
			return allTextures[front];
		else if (allTextures[back]->getName() == _NAME)
			return allTextures[back];
		else
			return nullptr;
	}
	else if (allTextures[mid]->getName() > _NAME)
	{
		return findTexture(front, mid, _NAME);
	}
	else
	{
		return findTexture(mid, back, _NAME);
	}
}

Mesh * ResourceManager::findMesh(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allMeshes[mid]->getName() == _NAME)
	{
		return allMeshes[mid];
	}
	else if (back - front <= 1)
	{
		if (allMeshes[front]->getName() == _NAME)
			return allMeshes[front];
		else if (allMeshes[back]->getName() == _NAME)
			return allMeshes[back];
		else
			return nullptr;
	}
	else if (allMeshes[mid]->getName() > _NAME)
	{
		return findMesh(front, mid, _NAME);
	}
	else
	{
		return findMesh(mid, back, _NAME);
	}
}

GameObject * ResourceManager::findGameObject(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allGameObjects[mid]->getName() == _NAME)
	{
		return allGameObjects[mid];
	}
	else if (back - front <= 1)
	{
		if (allGameObjects[front]->getName() == _NAME)
			return allGameObjects[front];
		else if (allGameObjects[back]->getName() == _NAME)
			return allGameObjects[back];
		else
			return nullptr;
	}
	else if (allGameObjects[mid]->getName() > _NAME)
	{
		return findGameObject(front, mid, _NAME);
	}
	else
	{
		return findGameObject(mid, back, _NAME);
	}
}

Light * ResourceManager::findLight(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (AllLight[mid]->getName() == _NAME)
	{
		return AllLight[mid];
	}
	else if (back - front <= 1)
	{
		if (AllLight[front]->getName() == _NAME)
			return AllLight[front];
		else if (AllLight[back]->getName() == _NAME)
			return AllLight[back];
		else
			return nullptr;
	}
	else if (AllLight[mid]->getName() > _NAME)
	{
		return findLight(front, mid, _NAME);
	}
	else
	{
		return findLight(mid, back, _NAME);
	}
}

ShaderProgram * ResourceManager::findShader(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (Shaders[mid]->getName() == _NAME)
	{
		return Shaders[mid];
	}
	else if (back - front <= 1)
	{
		if (Shaders[front]->getName() == _NAME)
			return Shaders[front];
		else if (Shaders[back]->getName() == _NAME)
			return Shaders[back];
		else
			return nullptr;
	}
	else if (Shaders[mid]->getName() > _NAME)
	{
		return findShader(front, mid, _NAME);
	}
	else
	{
		return findShader(mid, back, _NAME);
	}
}

Transform * ResourceManager::findEntity(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (Transforms[mid]->getName() == _NAME)
	{
		return Transforms[mid];
	}
	else if (back - front <= 1)
	{
		if (Transforms[front]->getName() == _NAME)
			return Transforms[front];
		else if (Transforms[back]->getName() == _NAME)
			return Transforms[back];
		else
			return nullptr;
	}
	else if (Transforms[mid]->getName() > _NAME)
	{
		return findEntity(front, mid, _NAME);
	}
	else
	{
		return findEntity(mid, back, _NAME);
	}
}

Camera * ResourceManager::findCamera(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (Cameras[mid]->getName() == _NAME)
	{
		return Cameras[mid];
	}
	else if (back - front <= 1)
	{
		if (Cameras[front]->getName() == _NAME)
			return Cameras[front];
		else if (Cameras[back]->getName() == _NAME)
			return Cameras[back];
		else
			return nullptr;
	}
	else if (Cameras[mid]->getName() > _NAME)
	{
		return findCamera(front, mid, _NAME);
	}
	else
	{
		return findCamera(mid, back, _NAME);
	}
}

Hitbox2D * ResourceManager::findHitbox(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allHitboxes[mid]->getName() == _NAME)
	{
		return allHitboxes[mid];
	}
	else if (back - front <= 1)
	{
		if (allHitboxes[front]->getName() == _NAME)
			return allHitboxes[front];
		else if (allHitboxes[back]->getName() == _NAME)
			return allHitboxes[back];
		else
			return nullptr;
	}
	else if (allHitboxes[mid]->getName() > _NAME)
	{
		return findHitbox(front, mid, _NAME);
	}
	else
	{
		return findHitbox(mid, back, _NAME);
	}
}

Framebuffer * ResourceManager::findFramebuffer(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allFramebuffers[mid]->getName() == _NAME)
	{
		return allFramebuffers[mid];
	}
	else if (back - front <= 1)
	{
		if (allFramebuffers[front]->getName() == _NAME)
			return allFramebuffers[front];
		else if (allFramebuffers[back]->getName() == _NAME)
			return allFramebuffers[back];
		else
			return nullptr;
	}
	else if (allFramebuffers[mid]->getName() > _NAME)
	{
		return findFramebuffer(front, mid, _NAME);
	}
	else
	{
		return findFramebuffer(mid, back, _NAME);
	}
}

Weapon * ResourceManager::findWeapon(unsigned int front, unsigned int back, std::string _NAME)
{
	unsigned int mid = (front + back) / 2;
	if (allWeapons[mid]->getName() == _NAME)
	{
		return allWeapons[mid];
	}
	else if (back - front <= 1)
	{
		if (allWeapons[front]->getName() == _NAME)
			return allWeapons[front];
		else if (allWeapons[back]->getName() == _NAME)
			return allWeapons[back];
		else
			return nullptr;
	}
	else if (allWeapons[mid]->getName() > _NAME)
	{
		return findWeapon(front, mid, _NAME);
	}
	else
	{
		return findWeapon(mid, back, _NAME);
	}
}

int ResourceManager::findGameObjectINGAME(unsigned int front, unsigned int back, GameObject * _ADR)
{
	unsigned int mid = (front + back) / 2;
	if (allGameObjectsINGAME[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (allGameObjectsINGAME[front] == _ADR)
			return front;
		else if (allGameObjectsINGAME[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if (allGameObjectsINGAME[mid] > _ADR)
	{
		return findGameObjectINGAME(front, mid, _ADR);
	}
	else
	{
		return findGameObjectINGAME(mid, back, _ADR);
	}
}

int ResourceManager::findLightINGAME(unsigned int front, unsigned int back, Light * _ADR)
{
	unsigned int mid = (front + back) / 2;
	if (AllLightINGAME[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (AllLightINGAME[front] == _ADR)
			return front;
		else if (AllLightINGAME[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if (AllLightINGAME[mid] > _ADR)
	{
		return findLightINGAME(front, mid, _ADR);
	}
	else
	{
		return findLightINGAME(mid, back, _ADR);
	}
}

int ResourceManager::findEntityINGAME(unsigned int front, unsigned int back, Transform * _ADR)
{
	unsigned int mid = (front + back) / 2;
	if (TransformsINGAME[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (TransformsINGAME[front] == _ADR)
			return front;
		else if (TransformsINGAME[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if (TransformsINGAME[mid] > _ADR)
	{
		return findEntityINGAME(front, mid, _ADR);
	}
	else
	{
		return findEntityINGAME(mid, back, _ADR);
	}
}

int ResourceManager::findCameraINGAME(unsigned int front, unsigned int back, Camera * _ADR)
{
	unsigned int mid = (front + back) / 2;
	if (CamerasINGAME[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (CamerasINGAME[front] == _ADR)
			return front;
		else if (CamerasINGAME[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if (CamerasINGAME[mid] > _ADR)
	{
		return findCameraINGAME(front, mid, _ADR);
	}
	else
	{
		return findCameraINGAME(mid, back, _ADR);
	}
}

int ResourceManager::findWeaponINGAME(unsigned int front, unsigned int back, Weapon * _ADR)
{
	unsigned int mid = (front + back) / 2;
	if (allWeaponsINGAME[mid] == _ADR)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (allWeaponsINGAME[front] == _ADR)
			return front;
		else if (allWeaponsINGAME[back] == _ADR)
			return back;
		else
			return -1;
	}
	else if (allWeaponsINGAME[mid] > _ADR)
	{
		return findWeaponINGAME(front, mid, _ADR);
	}
	else
	{
		return findWeaponINGAME(mid, back, _ADR);
	}
}

Texture * ResourceManager::searchForTexture(std::string _NAME)
{
	if (allTextures.size() == 0)
		return nullptr;
	else
		return findTexture(0, allTextures.size() - 1, _NAME);
}

Mesh * ResourceManager::searchForMesh(std::string _NAME)
{
	if (allMeshes.size() == 0)
		return nullptr;
	else
		return findMesh(0, allMeshes.size() - 1, _NAME);
}

GameObject * ResourceManager::searchForGameObject(std::string _NAME)
{
	if (allGameObjects.size() == 0)
		return nullptr;
	else
		return findGameObject(0, allGameObjects.size() - 1, _NAME);
}

Light * ResourceManager::searchForLight(std::string _NAME)
{
	if (AllLight.size() == 0)
		return nullptr;
	else
		return findLight(0, AllLight.size() - 1, _NAME);
}

ShaderProgram * ResourceManager::searchForShader(std::string _NAME)
{
	if (Shaders.size() == 0)
		return nullptr;
	else
		return findShader(0, Shaders.size() - 1, _NAME);
}

Transform * ResourceManager::searchForTransform(std::string _NAME)
{
	if (Transforms.size() == 0)
		return nullptr;
	else
		return findEntity(0, Transforms.size() - 1, _NAME);
}

Camera * ResourceManager::searchForCamera(std::string _NAME)
{
	if (Cameras.size() == 0)
		return nullptr;
	else
		return findCamera(0, Cameras.size() - 1, _NAME);
}

Hitbox2D * ResourceManager::searchForHitbox(std::string _NAME)
{
	if (allHitboxes.size() == 0)
		return nullptr;
	else
		return findHitbox(0, allHitboxes.size() - 1, _NAME);
}

Framebuffer * ResourceManager::searchForFramebuffer(std::string _NAME)
{
	if (allFramebuffers.size() == 0)
		return nullptr;
	else
		return findFramebuffer(0, allFramebuffers.size() - 1, _NAME);
}

Weapon * ResourceManager::searchForWeapon(std::string _NAME)
{
	if (allWeapons.size() == 0)
		return nullptr;
	else
		return findWeapon(0, allWeapons.size() - 1, _NAME);
}

void ResourceManager::destroyObjectINGAME(Transform * _OBJ)
{
	destroyChildrenINGAME(_OBJ);

	int iter = findEntityINGAME(0, TransformsINGAME.size() - 1, _OBJ);
	if (iter >= 0)
	{
		TransformsINGAME.erase(TransformsINGAME.begin() + iter);
	}
	
	if (Camera* camera = dynamic_cast<Camera*>(_OBJ))
	{
		iter = findCameraINGAME(0, CamerasINGAME.size() - 1, camera);
		if (iter >= 0)
		{
			CamerasINGAME.erase(CamerasINGAME.begin() + iter);
		}
	}

	if (Light* light = dynamic_cast<Light*>(_OBJ))
	{
		iter = findLightINGAME(0, AllLightINGAME.size() - 1, light);
		if (iter >= 0)
		{
			AllLightINGAME.erase(AllLightINGAME.begin() + iter);
		}
	}

	if (Weapon* _W = dynamic_cast<Weapon*>(_OBJ))
	{
		iter = findWeaponINGAME(0, allWeaponsINGAME.size() - 1, _W);
		if (iter >= 0)
		{
			allWeaponsINGAME.erase(allWeaponsINGAME.begin() + iter);
		}
	}

	if (GameObject* _GO = dynamic_cast<GameObject*>(_OBJ))
	{
		iter = findGameObjectINGAME(0, allGameObjectsINGAME.size() - 1, _GO);
		if (iter >= 0)
		{
			allGameObjectsINGAME.erase(allGameObjectsINGAME.begin() + iter);
		}
	}

	delete _OBJ;
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
	Camera* SUB = getCamera(_NAME);
	if (SUB)
	{
		Camera* SUB2 = new Camera;
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

Weapon * ResourceManager::getCloneOfWeapon(std::string _NAME)
{
	Weapon* SUB = getWeapon(_NAME);
	if (SUB)
	{
		Weapon* SUB2 = nullptr;
		if (SUB->TT == Transform::TransformType::TYPE_Mine)
			SUB2 = new Mine;
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
			_TF->addChild(SUB3);
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
		else
		{
			_TF2 = getCloneOfEntity(_TF2->getName());
			_TF->addChild(_TF2);
		}
	}
}

