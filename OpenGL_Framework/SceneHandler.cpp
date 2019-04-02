#include "SceneHandler.h"
#include <cstdlib>
#include <ctime>

std::vector<Scene*> SceneHandler::scenes;
Scene* SceneHandler::currentScene;

bool SceneHandler::initHandler(int windowWidth, int windowHeight)
{
	currentScene = nullptr;
	
	Scene::frameTimeCurrSample = 0;
	Scene::guiEnabled = true;

	Scene::windowWidth = windowWidth;
	Scene::windowHeight = windowHeight;

	Scene::aspect = (float)windowWidth / (float)windowHeight;

	for (int i = 0; i < Scene::frameTimeNumSamples; ++i)
		Scene::frameTimeSamples[i] = 0.016f;
	if (Scene::guiEnabled && !UI::isInit)
		UI::InitImGUI();

	Camera::init();

	Light::init();
	Light::_UBO.bind(3);

	Scene::uRes.allocateMemory(sizeof(vec4));
	Scene::uRes.bind(4);

	Scene::uniformBufferTime.allocateMemory(sizeof(float));
	Scene::uniformBufferTime.bind(1);

	ShaderProgram::initDefault();
	Framebuffer::initFrameBuffers();

	SAT_DEBUG_LOG("MESHES");
	std::string masterFile = "_MESH_MASTER_LIST";
	loadAllMeshes(masterFile);

	SAT_DEBUG_LOG("TEXTURES");
	masterFile = "_TEXTURE_MASTER_LIST";
	loadAllTextures(masterFile);

	SAT_DEBUG_LOG("LUTS");
	masterFile = "_LUT_MASTER_LIST";
	loadAllLUTS(masterFile);

	SAT_DEBUG_LOG("SOUNDS");
	masterFile = "_SOUNDS_MASTER_LIST";
	loadAllSounds(masterFile);

	SAT_DEBUG_LOG("LIGHTS");
	masterFile = "_LIGHTS_MASTER_LIST";
	loadAllLights(masterFile);

	SAT_DEBUG_LOG("HITBOXES");
	masterFile = "_HITBOX_MASTER_LIST";
	loadAllHitboxes(masterFile);

	SAT_DEBUG_LOG("FRAMEBUFFERS");
	masterFile = "_FRAMEBUFFER_MASTER_LIST";
	loadAllFramebuffers(masterFile);

	SAT_DEBUG_LOG("SHADERS");
	masterFile = "_SHADER_MASTER_LIST";
	loadAllShaders(masterFile);

	SAT_DEBUG_LOG("MATERIALS");
	Material::initDefaultTextures();
	masterFile = "_MATERIALS_MASTER_LIST";
	loadAllMaterials(masterFile);

	SAT_DEBUG_LOG("CAMERAS");
	masterFile = "_CAMERA_MASTER_LIST";
	loadAllCameras(masterFile);

	SAT_DEBUG_LOG("FONTS");
	masterFile = "_TEXT_MASTER_LIST";
	loadAllFonts(masterFile);

	SAT_DEBUG_LOG("OBJECTS");
	masterFile = "_OBJECT_MASTER_LIST";
	loadAllObjects(masterFile);

	Mine::weaponInit();
	Hammer::weaponInit();
	Axe::weaponInit();

	Powerup* BASEPOWER = new Powerup;
	BASEPOWER->setName("POWERUP");
	rm::addEntity(BASEPOWER);

	GameObject::initGameObjects();

	for (int i = 0; i < 4; i++)
	{
		if (Scene::_INPUT.controllerConnected(i))
			Scene::controllers.push_back(Scene::_INPUT.getController(i));
		else
			Scene::controllers.push_back(nullptr);
	}
	Stick s;
	for (int i = 0; i < 8; i++)
		Scene::playerInput.push_back(s);
	Triggers t;
	for (int i = 0; i < 4; i++)
		Scene::playerTriggers.push_back(t);

	//std::cout << Scene::aspect << std::endl;

	Scene::setDefaults();

	return true;
}

void SceneHandler::addScene(Scene * _S)
{
	protectedAddScene(_S);
}

void SceneHandler::destroyScene(Scene * _S)
{
	int SCENE = getLocationOfScene(_S);
	if (SCENE >= 0)
	{
		delete scenes[SCENE];
		scenes.erase(scenes.begin() + SCENE);
	}
}

void SceneHandler::destroyScene(std::string scene_name)
{
	int SCENE = getLocationOfScene(scene_name);
	if (SCENE >= 0)
	{
		delete scenes[SCENE];
		scenes.erase(scenes.begin() + SCENE);
	}
}

void SceneHandler::runScene(Scene * _S)
{
	currentScene = _S;
}

void SceneHandler::endScene(Scene * _S)
{
	if (currentScene == _S)
		currentScene = nullptr;
	_S->releaseScene();
}

void SceneHandler::pauseScene(Scene * _S)
{
	_S->pause(true);
}

void SceneHandler::resumeScene(Scene * _S)
{
	_S->pause(false);
}

void SceneHandler::replaceScene(Scene * _S)
{
	if (currentScene)
		currentScene->releaseScene();
	_S->initializeGame();
	runScene(_S);
}

Scene * SceneHandler::getCurrentScene()
{
	return currentScene;
}

void SceneHandler::update()
{
	//glutWarpPointer(Scene::windowWidth / 2, Scene::windowHeight / 2);
	//glutSetCursor(GLUT_CURSOR_NONE);
	Sound::engine.Update();

	if (currentScene->readyToTerminate)
		replaceScene(searchScenes(currentScene->nextScene));
	currentScene->update();
	Scene::mouseHandler();
	Scene::keyHandler();
}

void SceneHandler::draw()
{
	currentScene->draw();
}

void SceneHandler::GUI()
{
}

void SceneHandler::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	currentScene->keyboardDown(key, mouseX, mouseY);
}

void SceneHandler::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	currentScene->keyboardUp(key, mouseX, mouseY);
}

void SceneHandler::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	currentScene->keyboardSpecialDown(key, mouseX, mouseY);
}

void SceneHandler::keyboardSpecialUp(int key, int mouseX, int mouseY)
{
	currentScene->keyboardSpecialUp(key, mouseX, mouseY);
}

void SceneHandler::mouseClicked(int button, int state, int x, int y)
{
	currentScene->mouseClicked(button, state, x, y);
}

void SceneHandler::mouseMoved(int x, int y)
{
	currentScene->mouseMoved(x, y);
}

void SceneHandler::mousePassive(int x, int y)
{
	currentScene->mousePassive(x, y);
}

void SceneHandler::reshapeWindow(int w, int h)
{
	currentScene->reshapeWindow(w, h);
}

Scene* SceneHandler::searchScenes(Scene * _S)
{
	if (scenes.size() > 0)
	{
		binarySearchScenes(_S, 0, scenes.size() - 1);
	}
	else
	{
		return nullptr;
	}
}

Scene* SceneHandler::searchScenes(std::string _NAME)
{
	if (scenes.size() > 0)
	{
		return binarySearchScenes(_NAME, 0, scenes.size() - 1);
	}
	else
	{
		return nullptr;
	}
}

Scene* SceneHandler::binarySearchScenes(Scene * _S, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (scenes[mid] == _S)
	{
		return scenes[mid];
	}
	else if (back - front <= 1)
	{
		if (scenes[front] == _S)
			return scenes[front];
		else if (scenes[back] == _S)
			return scenes[back];
		else
			return nullptr;
	}
	else if (scenes[mid] > _S)
	{
		return binarySearchScenes(_S, front, mid);
	}
	else
	{
		return binarySearchScenes(_S, mid, back);
	}
}

Scene* SceneHandler::binarySearchScenes(std::string _NAME, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (scenes[mid]->getName() == _NAME)
	{
		return scenes[mid];
	}
	else if (back - front <= 1)
	{
		if (scenes[front]->getName() == _NAME)
			return scenes[front];
		else if (scenes[back]->getName() == _NAME)
			return scenes[back];
		else
			return nullptr;
	}
	else if (scenes[mid]->getName() > _NAME)
	{
		return binarySearchScenes(_NAME, front, mid);
	}
	else
	{
		return binarySearchScenes(_NAME, mid, back);
	}
}

int SceneHandler::getLocationOfScene(Scene * _S)
{
	if (scenes.size() > 0)
	{
		return getBinarySortLocationOfScene(_S, 0, scenes.size() - 1);
	}
	else
	{
		return -1;
	}
}

int SceneHandler::getLocationOfScene(std::string _NAME)
{
	if (scenes.size() > 0)
	{
		return getBinarySortLocationOfScene(_NAME, 0, scenes.size() - 1);
	}
	else
	{
		return -1;
	}
}

int SceneHandler::getBinarySortLocationOfScene(Scene * _S, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (scenes[mid] == _S)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (scenes[front] == _S)
			return front;
		else if (scenes[back] == _S)
			return back;
		else
			return -1;
	}
	else if (scenes[mid] > _S)
	{
		return getBinarySortLocationOfScene(_S, front, mid);
	}
	else
	{
		return getBinarySortLocationOfScene(_S, mid, back);
	}
}

int SceneHandler::getBinarySortLocationOfScene(std::string _NAME, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (scenes[mid]->getName() == _NAME)
	{
		return mid;
	}
	else if (back - front <= 1)
	{
		if (scenes[front]->getName() == _NAME)
			return front;
		else if (scenes[back]->getName() == _NAME)
			return back;
		else
			return -1;
	}
	else if (scenes[mid]->getName() > _NAME)
	{
		return getBinarySortLocationOfScene(_NAME, front, mid);
	}
	else
	{
		return getBinarySortLocationOfScene(_NAME, mid, back);
	}
}

void SceneHandler::protectedAddScene(Scene * _S)
{
	if (scenes.size() > 0)
	{
		protectedBinarySearchAndAddScene(_S, 0, scenes.size() - 1);
	}
	else
	{
		scenes.push_back(_S);
	}
}

void SceneHandler::loadAllTextures(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/textures/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Texture* tex = new Texture;
		if (!tex->load(temp + ".png"))
		{
			std::cout << temp << " texture could not load!" << std::endl;
			delete tex;
			tex = nullptr;
		}
		else
		{
			tex->setName(temp);
			ResourceManager::addTexture(tex);
		}
	}
	masterFile.close();
}

void SceneHandler::loadAllLUTS(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/LUTs/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Texture* tex = new Texture;
		if (!tex->load3D(temp + ".cube"))
		{
			std::cout << temp << " texture could not load!" << std::endl;
			delete tex;
			tex = nullptr;
		}
		else
		{
			tex->setName(temp);
			ResourceManager::addLUT(tex);
		}
	}
	masterFile.close();
}

void SceneHandler::loadAllMeshes(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/models/" + fileName + ".txt");
	std::string temp, dataparse;
	unsigned int instancing;
	while (std::getline(masterFile, temp))
	{
		std::getline(masterFile, dataparse);
		instancing = std::stoi(dataparse);
		Mesh* m = new Mesh;
		if (!m->LoadFromObj(temp + ".obj", instancing))
		{
			std::cout << temp << " model could not load!" << std::endl;
			delete m;
			m = nullptr;
		}
		else
		{
			m->setName(temp);
			//std::cout << temp << std::endl;
			ResourceManager::addMesh(m);
		}
	}
	masterFile.close();
}

void SceneHandler::loadAllObjects(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Objects/" + fileName + ".txt");
	std::ifstream objectFiles;
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		createChild(temp, nullptr);
	}
	masterFile.close();

	std::string fetch = "_PLAYER_MASTER_LIST";
	loadAllPlayerObjects(fetch);
}

void SceneHandler::loadAllUIElements(std::string & fileName)
{
}

void SceneHandler::loadAllLights(std::string & filename)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Lights/" + filename + ".txt");
	std::string temp;
	std::ifstream lightFiles;
	std::string dataHolder;
	vec4 dir;
	vec4 pos;
	vec4 dse;
	vec4 clq;
	vec4 col;
	dir.w = 0;
	pos.w = 1;
	dse.w = 0;
	col.w = 1;

	//SAT_DEBUG_LOG(filename.c_str());
	//unsigned int lightType = 0;
	while (std::getline(masterFile, temp))
	{
		//SAT_DEBUG_LOG(temp.c_str());

		Light* lot = new Light;
		lot->setName(temp);
		lightFiles.open("../assets/Lights/" + temp + ".txt");
		std::getline(lightFiles, dataHolder);
		if (dataHolder == "DIRECTIONAL")
			lot->type = Light::LightType::Directional;
		else if (dataHolder == "POINT")
			lot->type = Light::LightType::Point;
		else if (dataHolder == "SPOT")
			lot->type = Light::LightType::Spotlight;

		std::getline(lightFiles, dataHolder);
		dir.x = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		dir.y = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		dir.z = std::stof(dataHolder);

		std::getline(lightFiles, dataHolder);
		pos.x = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		pos.y = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		pos.z = std::stof(dataHolder);

		std::getline(lightFiles, dataHolder);
		dse.x = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		dse.y = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		dse.z = std::stof(dataHolder);

		std::getline(lightFiles, dataHolder);
		clq.x = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		clq.y = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		clq.z = std::stof(dataHolder);

		std::getline(lightFiles, dataHolder);
		col.x = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		col.y = std::stof(dataHolder);
		std::getline(lightFiles, dataHolder);
		col.z = std::stof(dataHolder);

		std::getline(lightFiles, dataHolder);
		col.w = std::stof(dataHolder);

		//Light* lot = new Light;// (dir, pos, dse, clq, col, temp, lightType);
		lot->color = col;
		lot->position = pos;
		lot->direction = dir;
		lot->constantAtten = clq.x;
		lot->linearAtten = clq.y;
		lot->quadAtten = clq.z;

		std::getline(lightFiles, dataHolder);
		if (dataHolder == "EMPTY")
		{
			lot->LIGHT_MESH = nullptr;
		}
		else
		{
			Mesh* _M = ResourceManager::getMesh(dataHolder);
			lot->LIGHT_MESH = _M;
		}

		ResourceManager::addEntity(lot);
		lightFiles.close();
	}
	masterFile.close();
}

void SceneHandler::loadAllHitboxes(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Hitboxes/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Hitbox2D* HB = new Hitbox2D;
		HB->name = temp;
		std::string DATBOI = "../assets/Hitboxes/" + temp + ".obj";
		HB->loadFromFile(DATBOI);
		ResourceManager::addHitbox(HB);
	}
	masterFile.close();
}

void SceneHandler::loadAllPlayerObjects(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Objects/" + fileName + ".txt");
	std::ifstream objectFiles;
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		createChild(temp, nullptr);
	}
	masterFile.close();
}

void SceneHandler::loadAllMorphTargs(std::string & fileName)
{
}

void SceneHandler::loadAllShaders(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/shaders/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		ShaderProgram* SP = new ShaderProgram;
		SP->setName(temp);
		//SAT_DEBUG_LOG(temp.c_str());
		std::string vertF, fragF;
		std::getline(masterFile, vertF);
		std::getline(masterFile, fragF);
		SP->load(vertF, fragF);
		ResourceManager::addShader(SP);
	}
	masterFile.close();
}

void SceneHandler::loadAllFramebuffers(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Framebuffers/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Framebuffer* _FB = new Framebuffer;
		_FB->setName(temp);
		std::ifstream fbFile;
		std::string dataparse;
		fbFile.open("../assets/Framebuffers/" + temp + ".fb");
		while (std::getline(fbFile, dataparse))
		{
			if (dataparse == "COLOR")
			{
				_FB->addColorTarget(GL_RGB8);
			}
			else if (dataparse == "DEPTH")
			{
				_FB->addDepthTarget();
			}
			//else if (dataparse == "DIMENSIONS")
			//{
			//	vec4 holder;
			//
			//	std::getline(fbFile, dataparse);
			//	holder.x = std::stof(dataparse);
			//	std::getline(fbFile, dataparse);
			//	holder.y = std::stof(dataparse);
			//	std::getline(fbFile, dataparse);
			//	holder.z = std::stof(dataparse);
			//	std::getline(fbFile, dataparse);
			//	holder.w = std::stof(dataparse);
			//
			//	_FB->setDimensions(holder);
			//}
		}

		_FB->init(Scene::windowWidth, Scene::windowHeight);
		ResourceManager::addFramebuffer(_FB);

		fbFile.close();
	}
	masterFile.close();
}

void SceneHandler::loadAllCameras(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Cameras/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Camera* _cam = new Camera;
		_cam->setName(temp);
		std::ifstream camFile;
		std::string dataparse;
		camFile.open("../assets/Cameras/" + temp + ".cam");

		std::getline(camFile, dataparse);
		if (dataparse == "PERSPECTIVE")
		{
			vec4 VP;

			std::getline(camFile, dataparse);
			VP.x = std::stof(dataparse);

			std::getline(camFile, dataparse);
			if (dataparse == "WINDOW")
				VP.y = Scene::aspect;
			else
				VP.y = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VP.z = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VP.w = std::stof(dataparse);

			_cam->perspective(VP.x, VP.y, VP.z, VP.w);
		}
		else
		{
			vec3 VO, VR;

			std::getline(camFile, dataparse);
			VO.x = std::stof(dataparse);

			std::getline(camFile, dataparse);
			if (dataparse == "WINDOW")
				VO.y = VO.x / Scene::aspect;
			else
				VO.y = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VO.z = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VR.x = std::stof(dataparse);

			std::getline(camFile, dataparse);
			if (dataparse == "WINDOW")
				VR.y = VR.x / Scene::aspect;
			else
				VR.y = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VR.z = std::stof(dataparse);

			_cam->orthographic(VO.x, VR.x, VO.y, VR.y, VO.z, VR.z);
		}

		std::getline(camFile, dataparse);
		Framebuffer* FB_TARG = ResourceManager::getFramebuffer(dataparse);
		if (FB_TARG)
			_cam->attachFrameBuffer(FB_TARG);

		ResourceManager::addEntity(_cam);
		camFile.close();

		//std::cout << _cam->getProjection() << std::endl;
	}
	masterFile.close();
}

void SceneHandler::loadAllMaterials(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/materials/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Material* mat = new Material(temp);
		mat->setName(temp);
		//std::cout << mat << ", " << temp << std::endl;
		ResourceManager::addMaterial(mat);
	}
	masterFile.close();
}

void SceneHandler::loadAllFonts(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/GameText/" + fileName + ".txt");
	std::string temp, temp2, temp3;
	//std::cout << fileName << std::endl;
	while (std::getline(masterFile, temp))
	{
		//std::cout << temp << std::endl;
		std::getline(masterFile, temp2);
		std::getline(masterFile, temp3);

		Text* _T = new Text;
		if (temp3 == "p")
		{
			_T->init(temp, temp2, "TEXT_SHADER");
			//std::cout << "HERE!" << std::endl;
		}
		else if (temp3 == "r")
		{
			_T->init(temp, temp2, "TEXT_ROT_SHADER");
			//std::cout << "YAY!" << std::endl;
		}
		else
		{
			_T->init(temp, temp2, "TEXT_UI");
			//std::cout << "AUUUUUUGGGHGHH!" << std::endl;
		}

		_T->setName(temp);

		//std::cout << _T->getName() << std::endl;

		ResourceManager::addEntity(_T);
	}
	masterFile.close();
}

void SceneHandler::loadAllSounds(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("../assets/Sounds/" + fileName + ".txt");
	std::string temp, temp2, temp3, temp4;
	//std::cout << fileName << std::endl;
	while (std::getline(masterFile, temp))
	{
		std::getline(masterFile, temp2);
		std::getline(masterFile, temp3);
		std::getline(masterFile, temp4);

		Sound* _S = new Sound;
		_S->setName(temp);

		bool lop = false;
		bool itd = false;
		if (temp3 == "true")
			lop = true;
		if (temp4 == "true")
			itd = true;

		std::string TEMP = "../assets/Sounds/" + temp2 + ".wav";
		_S->Load(TEMP.c_str(), itd, lop);
		ResourceManager::addSound(_S);
	}
	masterFile.close();
}

void SceneHandler::createChild(std::string & fileName, Transform * parent)
{
	std::ifstream objectFiles;
	objectFiles.open("../assets/Objects/" + fileName + ".txt");
	std::string meshName, textureName, BRSTR, objType;
	std::string dataparse;
	float boundRadius;
	std::getline(objectFiles, meshName);
	std::getline(objectFiles, textureName);
	std::getline(objectFiles, objType);
	std::getline(objectFiles, BRSTR);
	//std::cout << fileName << std::endl;
	boundRadius = std::stof(BRSTR);
	bool hasTex = false, hasMesh = false;

	GameObject* obj; //= new GameObject;
	obj = ResourceManager::searchForGameObject(fileName);
	bool exists = false;

	if (obj)
	{
		exists = true;
	}

	if (!exists)
	{
		if (objType == "BASE_PLATE")
		{
			obj = new BasePlate;
		}
		else if (objType == "PLAYER")
		{
			obj = new Player;
		}
		else if (objType == "BOUNDARY")
		{
			obj = new Boundary;
		}
		else if (objType == "DESTRUCTABLE")
		{
			obj = new Destructable;
		}
		else if (objType == "RECOLOR")
		{
			obj = new RecolorObject;
		}
		else
		{
			obj = new GameObject;
		}

		if (textureName == "EMPTY")
		{
			obj->setMaterial(nullptr);
			hasTex = true;
		}
		else
		{
			Material* _MAT = rm::getMaterial(textureName);
			//std::cout << _MAT << std::endl;
			if (_MAT)
			{
				obj->setMaterial(_MAT);
				hasTex = true;
			}
		}
		if (meshName == "EMPTY")
		{
			obj->setMesh(nullptr);
			hasMesh = true;
		}
		else
		{
			Mesh* _MESH = rm::getMesh(meshName);
			if (_MESH)
			{
				obj->setMesh(_MESH);
				hasMesh = true;
			}
		}

		obj->setBoundingRadius(boundRadius);
		if (!hasTex || !hasMesh)
		{
			std::cout << fileName << " game object could not load!" << std::endl;
			if (!hasTex)
				std::cout << "Material not found!" << std::endl;
			if (!hasMesh)
				std::cout << "Mesh not found!" << std::endl;
			delete obj;
			obj = nullptr;
		}
		else
		{
			obj->setName(fileName);
			ResourceManager::addEntity(obj);
			while (std::getline(objectFiles, dataparse))
			{
				if (dataparse == "OBJECT")
				{
					std::getline(objectFiles, dataparse);

					createChild(dataparse, obj);
					Transform* _GO = obj->getChildren().at(obj->getChildren().size() - 1);
					std::string popOut;

					vec3 nPos;
					vec3 nRot;
					float nScale;

					std::getline(objectFiles, popOut);
					nPos.x = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nPos.y = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nPos.z = std::stof(popOut);

					std::getline(objectFiles, popOut);
					nRot.x = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nRot.y = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nRot.z = std::stof(popOut);

					std::getline(objectFiles, popOut);
					nScale = std::stof(popOut);

					_GO->setLocalPos(nPos);
					_GO->setLocalRot(nRot);
					_GO->setScale(nScale);
				}
				else if (dataparse == "LIGHT")
				{
					std::getline(objectFiles, dataparse);

					Light* LIT = rm::getLight(dataparse);
					if (LIT)
					{
						obj->addChild(LIT);
					}
				}
				else if (dataparse == "CAMERA")
				{
					std::getline(objectFiles, dataparse);

					Camera* CAM = rm::getCamera(dataparse);

					std::string popOut;

					vec3 nPos;
					vec3 nRot;
					float nScale;

					std::getline(objectFiles, popOut);
					nPos.x = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nPos.y = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nPos.z = std::stof(popOut);

					std::getline(objectFiles, popOut);
					nRot.x = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nRot.y = std::stof(popOut);
					std::getline(objectFiles, popOut);
					nRot.z = std::stof(popOut);

					std::getline(objectFiles, popOut);
					nScale = std::stof(popOut);

					CAM->setLocalPos(nPos);
					CAM->setLocalRot(nRot);
					CAM->setScale(nScale);
				}
				else if (dataparse == "PHYS")
				{
					std::getline(objectFiles, dataparse);
					Hitbox2D* _HB = ResourceManager::searchForHitbox(dataparse);
					if (_HB)
					{
						PhysicsBody _PB(_HB);
						obj->setPhysicsBody(_PB);
					}
					std::getline(objectFiles, dataparse);
					if (dataparse == "true")
						obj->getPhysicsBody()->getHB()->dynamic = true;
					else
						obj->getPhysicsBody()->getHB()->dynamic = false;
					if (dataparse == "grass")
						obj->getPhysicsBody()->getHB()->grass = true;
					if (dataparse == "INF")
						obj->getPhysicsBody()->getHB()->unbreakable = true;
				}
				else if (dataparse == "TEXTURE")
				{
					std::getline(objectFiles, dataparse);
					Texture* _TEX = ResourceManager::searchForTexture(dataparse);
					if (_TEX)
					{
						obj->addTexture(_TEX);
					}
				}
				else if (dataparse == "POINTS")
				{
					std::getline(objectFiles, dataparse);
					obj->destrPoints = std::stoi(dataparse);
					//std::cout << dataparse << std::endl;
				}
			}
		}
	}
	objectFiles.close();
	if (parent)
		parent->addChild(obj);
}

void SceneHandler::protectedBinarySearchAndAddScene(Scene * _S, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (scenes[mid]->getName() == _S->getName())
	{
		SAT_DEBUG_LOG("ERROR: %s SCENE ALREADY ADDED", _S->getName().c_str());
	}
	else if (back - front <= 1)
	{
		if (scenes[front]->getName() > _S->getName())
			scenes.insert(scenes.begin() + front, _S);
		else if (scenes[back]->getName() > _S->getName())
			scenes.insert(scenes.begin() + back, _S);
		else
			scenes.insert(scenes.begin() + back + 1, _S);
	}
	else if (scenes[mid]->getName() > _S->getName())
	{
		protectedBinarySearchAndAddScene(_S, front, mid);
	}
	else
	{
		protectedBinarySearchAndAddScene(_S, mid, back);
	}
}
