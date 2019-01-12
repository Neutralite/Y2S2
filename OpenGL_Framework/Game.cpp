#include "Game.h"
#include "KeyDefines.h"
#include "UTILITY.h"

//TODO: Binary Mesh, Shader loaders
//TODO: uniformBufferObjects


const char* vertexShaderSource =
"#version 330\n"
"layout(location = 0) in vec3 mesh_position;\n"
"uniform mat4 u_mvp;\n"
"void main()\n"
"{\n"
"	gl_Position = u_mvp * vec4(mesh_position, 1.0);\n"
"}\n";

const char* fragmentShaderSource =
"#version 330\n"
"out vec4 pixelColor;\n"
"void main() { pixelColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); }\n";

#define DEG_TO_RAD 3.14159f / 180.f;
#define RAD_TO_DEG 180.f / 3.14159f;

Game::Game() : FB(1), WorkBuffer1(1), WorkBuffer2(1), testBuff(1), UISCREEN(1)
{
}
//, theMap(std::string("Assets/Map/TEST_MAP.txt"), &objects, 100, 100, 6.f, 6.f, BASE_PLATE)
Game::~Game()
{
	delete updateTimer;

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] != nullptr)
		{
			delete objects[i];
			objects[i] = nullptr;
		}
	}
	for (int i = 0; i < meshList.size(); i++)
	{
		if (meshList[i] != nullptr)
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i] != nullptr)
		{
			delete textures[i];
			textures[i] = nullptr;
		}
	}
	for (int i = 0; i < baseTextures.size(); i++)
	{
		if (baseTextures[i] != nullptr)
		{
			delete baseTextures[i];
			baseTextures[i] = nullptr;
		}
	}
	for (int i = 0; i < postProcEffects.size(); i++)
	{
		if (postProcEffects[i] != nullptr)
		{
			postProcEffects[i]->Unload();
			postProcEffects[i] = nullptr;
		}
	}
	for (int i = 0; i < UIELEM.size(); i++)
	{
		if (UIELEM[i] != nullptr)
		{
			delete UIELEM[i];
			UIELEM[i] = nullptr;
		}
	}
	for (int i = 0; i < allLight.size(); i++)
	{
		if (allLight[i] != nullptr)
		{
			delete allLight[i];
			allLight[i] = nullptr;
		}
	}
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] != nullptr)
		{
			delete players[i];
			players[i] = nullptr;
		}
	}
	//texture->unload();
	//...
}

void Game::initializeGame()
{
	for (int i = 0; i < 1; i++)
	{
		Object *p = new Player;
		players.push_back(p);
	}

	//Object* testObj = new Object();
	//Object* testObj2 = new Object();
	//Object* testObj3 = new Object();
	std::string nameOfFile = "_MESH_MASTER_LIST";
	loadAllMeshes(nameOfFile);
	nameOfFile = "_MORPH_MASTER_LIST";
	loadAllMorphTargs(nameOfFile);
	nameOfFile = "_TEXTURE_MASTER_LIST";
	loadAllTextures(nameOfFile);
	nameOfFile = "_LIGHTS_MASTER_LIST";
	loadAllLights(nameOfFile);
	nameOfFile = "_HITBOX_MASTER_LIST";
	loadAllHitboxes(nameOfFile);
	nameOfFile = "_OBJECT_MASTER_LIST";
	loadAllObjects(nameOfFile);
	nameOfFile = "_UI_MASTER_LIST";
	loadAllUIElements(nameOfFile);

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->getIdentity() == "MINE")
		{
			MINE_OBJ = objects[i];
		}
		else if (objects[i]->getIdentity() == "EXPLOSION")
		{
			EXP_OBJ = objects[i];
		}
	}
	//for (int i = 0; i < objects.size(); i++)
	//{
	//	if (objects[i]->getIdentity() == "TOP_WALL")
	//	{
	//		TOP_WALL = objects[i];
	//		TOP_WALL->setPosition(vec3(297.f, 0, -9.f));
	//		TOP_WALL->setRotation(vec3(0, 0, 0));
	//	}
	//	else if (objects[i]->getIdentity() == "BOTTOM_WALL")
	//	{
	//		BOTTOM_WALL = objects[i];
	//		BOTTOM_WALL->setPosition(vec3(297.f, 0, 603.f));
	//		BOTTOM_WALL->setRotation(vec3(0, 0, 0));
	//	}
	//	else if (objects[i]->getIdentity() == "LEFT_WALL")
	//	{
	//		LEFT_WALL = objects[i];
	//		LEFT_WALL->setPosition(vec3(-9.f, 0, 297.f));
	//		LEFT_WALL->setRotation(vec3(0, 0, 0));
	//	}
	//	else if (objects[i]->getIdentity() == "RIGHT_WALL")
	//	{
	//		RIGHT_WALL = objects[i];
	//		RIGHT_WALL->setPosition(vec3(603.f, 0, 297.f));
	//		RIGHT_WALL->setRotation(vec3(0, 0, 0));
	//	}
	//}

	SUN = allLight[0];

	//BASE_PLATE = new Object();
	//texture = new Texture();
	//baseTextures.push_back(texture);
	//TextureLayer* _TL1 = new TextureLayer;
	//_TL1->setTexture(texture);
	//textures.push_back(_TL1);
	//Mesh* mesh = new Mesh();
	//Mesh* mesh2 = new Mesh();
	//meshList.push_back(mesh);
	//meshList.push_back(mesh2);
	//BASE_PLATE->AttachMesh(mesh2);
	//BASE_PLATE->addTexture(_TL1);
	//BASE_PLATE->setIdentity("SAND");
	BASE_PLATE_SAND = objects[0];
	BASE_PLATE_T = objects[1];
	BASE_PLATE_ROAD = objects[2];
	BASE_PLATE_CORNER = objects[3];
	BASE_PLATE_4WAY = objects[4];
	BASE_PLATE_GRASS = objects[5];
	BASE_PLATE_DIRT = objects[6];
	BASE_PLATE_CONCRETE = objects[7];

	HEALTH_BAR = UIELEM[0];
	POINTS_BAR = UIELEM[1];
	TIME_BAR = UIELEM[2];
	SPED_BAR = UIELEM[3];

	//testObj->AttachMesh(mesh);
	//testObj2->AttachMesh(mesh);
	//objects.push_back(testObj);
	//testObj3->AttachMesh(mesh);
	//objects[0]->AttachObject(testObj2);
	//objects[0]->getChild(0)->AttachObject(testObj3);

	//Make sure depth works
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//VAO = Vertex Array Object (The place that boxes the cookies)
	//VBO = Vertex Buffer Object (The place that holds all the cookie models)
	updateTimer = new Timer();

	initFullscreenQuad();

	int success = GL_FALSE;

	ShaderProgram::initDefault();
	if (!PassThrough.Load("Assets/Shaders/StaticGeo.vert", "Assets/Shaders/DynamicLights.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!EXPLOSIONSHADER.Load("Assets/Shaders/EXPLODE.vert", "Assets/Shaders/EXPLODE.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!MINESHADER.Load("Assets/Shaders/EXPLODE.vert", "Assets/Shaders/MINE.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	//if (!PLAYA.Load("Assets/Shaders/Player.vert", "Assets/Shaders/DynamicLights.frag"))
	//{
	//	std::cout << "Shaders failed to initialize\n";
	//}

	if (!UI_SHADER.Load("Assets/Shaders/UIgeo.vert", "Assets/Shaders/BASIC.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!GreyScalePost.Load("Assets/Shaders/GenericPost.vert", "Assets/Shaders/GreyScalePost.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!BloomHP.Load("Assets/Shaders/GenericPost.vert", "Assets/Shaders/Bloom/BloomHighPass.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!BloomVB.Load("Assets/Shaders/GenericPost.vert", "Assets/Shaders/Bloom/BloomBlurVertical.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!BloomHB.Load("Assets/Shaders/GenericPost.vert", "Assets/Shaders/Bloom/BloomBlurHorizontal.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}

	if (!BloomCOMP.Load("Assets/Shaders/GenericPost.vert", "Assets/Shaders/Bloom/BloomComposite.frag"))
	{
		std::cout << "Shaders failed to initialize\n";
	}



	//if (!mesh->LoadFromFile("Assets/Models/Monkey.obj"))
	//{
	//	std::cout << "Mesh couldn't be retrieved from file\n";
	//}
	//if(!mesh2->LoadFromFile("Assets/Models/TESTING_BASE_PLATE.obj"))
	//{
	//	std::cout << "Mesh couldn't be retrieved from file\n";
	//}
	//
	//if (!texture->load("SAND.png"))
	//{
	//	std::cout << "\nTextures get an F- in loading\n";
	//	//system("pause");
	//	//exit(0);
	//}

	FB.InitDepthTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	FB.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!FB.CheckFBO())
	{
		std::cout << "All your FBO is belong to us\n";
		system("pause");
		exit(0);
	}

	UISCREEN.InitDepthTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	UISCREEN.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!UISCREEN.CheckFBO())
	{
		std::cout << "All your FBO is belong to us\n";
		system("pause");
		exit(0);
	}

	testBuff.InitDepthTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	testBuff.InitColorTexture(0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!testBuff.CheckFBO())
	{
		std::cout << "All your FBO is belong to us\n";
		system("pause");
		exit(0);
	}

	WorkBuffer1.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer1.CheckFBO())
	{
		std::cout << "All your FBO is belong to us\n";
		system("pause");
		exit(0);
	}

	WorkBuffer2.InitColorTexture(0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!WorkBuffer2.CheckFBO())
	{
		std::cout << "All your FBO is belong to us\n";
		system("pause");
		exit(0);
	}

	//Aspect ratio
	float aspect = 800.f / 432.f;

	//Set the camera's specs - the 'perspective frustrum'
	camera.perspective(60.0f, aspect, 1.0f, 1000.f);
	UIcam.orthographic(0, camera.ASPECT, 0, 1, -1000, 1000);
	UIcam.m_pLocalPosition = vec3(0, 0, 0);
	UIcam.update(DT);
	camera.m_pLocalPosition = vec3(0.f, cameraHeight, cameraHeight / sqrt(3));
	camera.setRotationAngleX(-60.f);
	//camera.setRotationAngleX(0.f);
	//camera.setRotationAngleY(-60.f);

	currentGoal = camera.getPosition();
	//camera.setRotationAngleX(-45.f);
	//objects[0]->setPosition(vec3(-4.f, 0.f, 0.f));
	//objects[0]->getChild(0)->setPosition(vec3(-4.f, 0.f, 0.f));
	for (int i = 0; i < 256; i++)
	{
		keysDown[i] = false;
		backCheckKeysDown[i] = false;
	}

	for (int i = 0; i < 10; i++)
	{
		mouseDown[i] = false;
		backCheckMouseDown[i] = false;
	}

	players[0]->setCam(&camera);
	players[0]->getPhysicsBody()->velocityLimit = 25.f;
	players[0]->getPhysicsBody()->baseAcc = 100.f;
	players[0]->getPhysicsBody()->baseTorque = 0.f;
	players[0]->getPhysicsBody()->angVelocityLimit = 0.f;
	players[0]->getPhysicsBody()->friction = 40.f;

	//placeAndTime.push_back(vec4(1, 1, 1, 1));
	mousePosition = 0.5f * vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
	prevMousePosition = mousePosition;
	setCursorVisible(true);

	std::vector<Object*> shipToField;
	shipToField.push_back(BASE_PLATE_SAND);
	shipToField.push_back(BASE_PLATE_T);
	shipToField.push_back(BASE_PLATE_ROAD);
	shipToField.push_back(BASE_PLATE_CORNER);
	shipToField.push_back(BASE_PLATE_4WAY);
	shipToField.push_back(BASE_PLATE_GRASS);
	shipToField.push_back(BASE_PLATE_DIRT);
	shipToField.push_back(BASE_PLATE_CONCRETE);


	HEALTH_BAR->setRotation(vec3(-90.f, 0, 90.f));
	POINTS_BAR->setRotation(vec3(-7.f, 0, 0));
	TIME_BAR->setRotation(vec3(-90.f, 0, 90.f));
	SPED_BAR->setRotation(vec3(-90.f, 0, 0));

	HEALTH_BAR->setPosition(vec3(0, 0, 0));
	HEALTH_BAR->getOrientation()->setScale(0.78f);
	TIME_BAR->setPosition(vec3(camera.ASPECT * 0.5f, 0, 0));
	//TIME_BAR->getOrientation()->setScale(0.6f);
	POINTS_BAR->setPosition(vec3(camera.ASPECT - 0.24f, 0, 0));
	POINTS_BAR->getOrientation()->setScale(1.2f);
	SPED_BAR->setPosition(vec3(camera.ASPECT- 0.15f, 0.85f, 0));
	SPED_BAR->getOrientation()->setScale(1.0f);

	for (int i = 0; i < UIELEM.size(); i++)
		UIELEM[i]->updateTransforms(DT);

	theMap = new Field(std::string("TEST_MAP.txt"), &objects, 100, 100, tileSize, tileSize, &shipToField, &players);
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
		{
			theMap->getSection(j, i)->getPlane()->updateTransforms(DT);
			for (int k = 0; k < theMap->getSection(j, i)->getNumObjOnFace(); k++)
			{
				Object* OOO = theMap->getSection(j, i)->getObjectOnFace(k);
				OOO->updateTransforms(DT);
				if (OOO->getChild(0)->getPhysicsBody()->collider != nullptr)
				{
					if (OOO->getChild(0)->getPhysicsBody()->collider->dynamic)
					{
						dynamicCollisions.push_back(OOO);
					}
					else
					{
						staticCollisions.push_back(OOO);
					}
				}
				OOO->setBaseRotation(OOO->getOrientation()->getRotationAngle());
				OOO->setBasePosition(OOO->getOrientation()->getPosition());
			}
		}

	players[0]->setBezTimeToHalf();

	//players[0]->updateTransforms(DT);
	camera.m_pLocalPosition = vec3(0.f, cameraHeight, cameraHeight / sqrt(3)) + players[0]->getOrientation()->getPosition();
	currentGoal = camera.getPosition();
	vec2 PPC = vec2(players[0]->getOrientation()->getPosition().x, players[0]->getOrientation()->getPosition().z) / tileSize;
	pcX = PPC.x + 0.5f;
	pcY = PPC.y + 0.5f;
	spawnPoint = players[0]->getOrientation()->getPosition();
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	DT = deltaTime;

	//rot1.setRotationAngleY(TotalGameTime * 10.f);
	//rot2.setRotationAngleX(sin(TotalGameTime * 5.f) * 25.f);
	//rot1.update(deltaTime);
	//rot2.update(deltaTime);
	//players[0]->getOrientation()->setRotationAngleX(TotalGameTime * 90.f);

	if (!paused)
	{
		TotalGameTime += deltaTime;

		//objects[0]->setRotation(objects[0]->getOrientation()->getRotationAngle() + vec3(0.0f, 10.0f, 0.0f) * deltaTime);
		//objects[0]->getChild(0)->setPosition(vec3(10.f * sin(TotalGameTime * 2.f), 0.f, 0.f));
		//objects[0]->getChild(0)->RotateBy(vec3(60.f * DT, 0, 0));
		//objects[0]->getChild(0)->getChild(0)->setPosition(vec3(0, 3.f * sin(TotalGameTime * 0.7f), 0));
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->updateTransforms(deltaTime);
		}
	}

	//vec3 cameraGoal = vec3(0, 0, 0);
	float cy = camera.getRotationAngleY() * DEG_TO_RAD;
	if (keysDown['A' - 1] || keysDown['a' - 1])
	{
		//cameraGoal.x -= cos(cy);
		//cameraGoal.z += sin(cy);
		players[0]->giveInput('L', true);
	}
	else
	{
		players[0]->giveInput('L', false);
	}
	if (keysDown['S' - 1] || keysDown['s' - 1])
	{
		//cameraGoal.x += sin(cy);
		//cameraGoal.z += cos(cy);
		players[0]->giveInput('D', true);
	}
	else
	{
		players[0]->giveInput('D', false);
	}
	if (keysDown['D' - 1] || keysDown['d' - 1])
	{
		//cameraGoal.x += cos(cy);
		//cameraGoal.z -= sin(cy);
		players[0]->giveInput('R', true);
	}
	else
	{
		players[0]->giveInput('R', false);
	}
	if (keysDown['W' - 1] || keysDown['w' - 1])
	{
		//cameraGoal.x -= sin(cy);
		//cameraGoal.z -= cos(cy);
		players[0]->giveInput('U', true);
	}
	else
	{
		players[0]->giveInput('U', false);
	}

	if (keysDown['`' - 1] || keysDown['~'])
	{
		PassThrough.reload();
	}

	//if (cameraGoal.Length() > 0)
	//	cameraGoal = cameraGoal.GetNormalized();
	////std::cout << cameraGoal.Length() << std::endl;
	//cameraGoal *= 30.f * DT;

	//std::cout << cy << std::endl;
	
	//currentGoal += cameraGoal;
	//std::cout << camera.getPosition().y << std::endl;


	while (camera.getRotationAngleY() - players[0]->getOrientation()->getRotationAngleY() > 180.f)
	{
		camera.setRotationAngleY(camera.getRotationAngleY() - 360.f);
	}
	while (camera.getRotationAngleY() - players[0]->getOrientation()->getRotationAngleY() < -180.f)
	{
		camera.setRotationAngleY(camera.getRotationAngleY() + 360.f);
	}
	float Ysub = LP::LERP(camera.getRotationAngleY(), players[0]->getOrientation()->getRotationAngleY(), 1.f - pow(0.9f, 60.f * DT));

	camera.setRotationAngleY(LP::LERP(camera.getRotationAngleY(), players[0]->getOrientation()->getRotationAngleY(), 1.f - pow(0.9f, 60.f * DT)));
	camera.setPosition(players[0]->getOrientation()->getPosition() + vec3(
		cameraHeight / sqrt(3) * sin(/*ToRadians(players[0]->getOrientation()->getRotationAngleY())*/ ToRadians(Ysub)), 
		cameraHeight, 
		cameraHeight / sqrt(3) * cos(/*ToRadians(players[0]->getOrientation()->getRotationAngleY())*/ ToRadians(Ysub))));

	//vec4 column1 = camera.getLocalToWorldMatrix().GetInverse() * vec4(1, 0, 0, 0);
	//vec4 column2 = camera.getLocalToWorldMatrix().GetInverse() * vec4(0, 1, 0, 0);
	//vec4 column3 = camera.getLocalToWorldMatrix().GetInverse() * vec4(0, 0, 1, 0);
	//vec4 column4 = camera.getLocalToWorldMatrix().GetInverse() * vec4(0, 0, 0, 1);
	//
	//std::cout << column1.x << ", " << column2.x << ", " << column3.x << ", " << column4.x << "\n"
	//	<< column1.y << ", " << column2.y << ", " << column3.y << ", " << column4.y << "\n"
	//	<< column1.z << ", " << column2.z << ", " << column3.z << ", " << column4.z << "\n"
	//	<< column1.w << ", " << column2.w << ", " << column3.w << ", " << column4.w << std::endl;

	//camera.setPosition(players[0]->getOrientation()->getPosition() + vec3(
	//	0,
	//	cameraHeight,
	//	cameraHeight / sqrt(3) * cos(ToRadians(players[0]->getOrientation()->getRotationAngleY()))));

	//std::cout << camera.getPosition().y << std::endl;
	//camera.setPosition(players[0]->getOrientation()->getPosition() + vec3(0, cameraHeight, 0));

	//std::cout << (camera.getPosition() - players[0]->getOrientation()->getPosition()).Length() << std::endl;

	//std::cout << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << ", " << std::endl;
	//camera.setPosition(players[0]->getOrientation()->getPosition() + vec3(0, cameraHeight, cameraHeight / sqrt(3)));

	//std::cout << camera.getRotationAngle().x << ", " << camera.getRotationAngle().y << ", " << camera.getRotationAngle().z << ", " << std::endl;
	//currentGoal = players[0]->getOrientation()->getPosition() + vec3(0, cameraHeight, cameraHeight / sqrt(3));
	//
	//camera.setChasePosition(currentGoal);
	//camera.camChase(DT);

	DeltaMousePos = 0.5f * vec2(WINDOW_WIDTH, WINDOW_HEIGHT) - mousePosition;

	prevMousePosition = mousePosition;

	//camera.update(deltaTime);

	//vec3 oldPos = players[0]->getOrientation()->getPosition();
	//std::cout << players[0]->getPhysicsBody()->velocity.Length() << std::endl;
	//std::cout << players[0]->getPhysicsBody()->position.x << " ----- " << players[0]->getPhysicsBody()->position.z << std::endl;

	if ((backCheckKeysDown['`' - 1] && !keysDown['`' - 1]) || (backCheckKeysDown['~' - 1] && !keysDown['~' - 1]))
	{
		players[0]->setPosition(spawnPoint);
	}
	if ((backCheckKeysDown['z' - 1] && !keysDown['z' - 1]) || (backCheckKeysDown['Z' - 1] && !keysDown['Z' - 1]))
	{
		resetMap();
	}
	//std::cout << players[0]->getPhysicsBody()->velocity.x << ", " << players[0]->getPhysicsBody()->velocity.y << ", " << players[0]->getPhysicsBody()->velocity.z << ", " << std::endl;
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->updateTransforms(DT);
		players[i]->setBezPointOut(*(players[i]->getTilt()) * -0.1f, *(players[i]->getTilt()) * -0.1f);
		//std::cout << players[i]->getTilt()->x << ", " << players[i]->getTilt()->y << ", " << players[i]->getTilt()->z << std::endl;
	}
	//std::cout << players[0]->getPhysicsBody()->velocity.x << ", " << players[0]->getPhysicsBody()->velocity.y << ", " << players[0]->getPhysicsBody()->velocity.z << ", " << std::endl;
	//std::cout << players[0]->getPhysicsBody()->velocity.Length() << std::endl;
	//std::cout << players[0]->getPhysicsBody()->position.x << " ----- " << players[0]->getPhysicsBody()->position.z << std::endl;

	vec2 PPU = vec2(players[0]->getOrientation()->getPosition().x, players[0]->getOrientation()->getPosition().z) / tileSize;
	int plX = (int) (PPU.x + 0.5f);
	int plY = (int) (PPU.y + 0.5f);
	//std::cout << plX << ", " << plY << std::endl;

	if (plX >= 0 && plX < 100 && plY >= 0 && plY < 100)
	{
		theMap->getSection(pcX, pcY)->removeFromFace(players[0]);
		theMap->getSection(plX, plY)->setOnFace(players[0]);
		pcX = plX;
		pcY = plY;
	}
	//else
	//{
	//	std::cout << plX << ", " << plY << std::endl;
	//}

	//for (int i = 0; i < theMap->getSection(0, 0)->getNumObjOnFace(); i++)
	//{
	//	std::cout << theMap->getSection(0, 0)->getObjectOnFace(i)->getChild(0)->getIdentity() << std::endl;
	//}
	
	//vec3 newPos = players[0]->getOrientation()->getPosition() - oldPos;

	//std::cout << newPos.Length() << std::endl;
	//std::cout << players[0]->getPhysicsBody()->velocity.x << ", " << players[0]->getPhysicsBody()->velocity.z << " --- " << DT << std::endl;

	//camera.setRotationAngle(camera.getRotationAngle() + vec3(DeltaMousePos.y, DeltaMousePos.x, 0) * 0.3f);
	//if (camera.getRotationAngleX() > 90)
	//{
	//	camera.setRotationAngleX(90);
	//}
	//else if (camera.getRotationAngleX() < -90)
	//{
	//	camera.setRotationAngleX(-90);
	//}

	for (int i = placeAndTime.size() - 1; i >= 0; i--)
	{
		placeAndTime[i].w -= DT;
		if (placeAndTime[i].w <= 0)
			placeAndTime.erase(placeAndTime.begin() + i);
	}

	camera.update(DT);

	if (backCheckKeysDown['1' - 1] && !keysDown['1' - 1])
	{
		normalRenderActive = !normalRenderActive;
	}
	if (backCheckKeysDown['2' - 1] && !keysDown['2' - 1])
	{
		bloomActive = !bloomActive;
	}
	if (backCheckKeysDown['3' - 1] && !keysDown['3' - 1])
	{
		rippleActive = !rippleActive;
	}
	if ((backCheckKeysDown['p' - 1] && !keysDown['p' - 1]) || (backCheckKeysDown['P' - 1] && !keysDown['P' - 1]))
	{
		//paused = !paused;
	}
	if ((backCheckKeysDown[' ' - 1] && !keysDown[' ' - 1]) && currentCoolDown <= 0.f)
	{
		Object* MINEobj = new Object;
		MINEobj->setPosition(players[0]->getOrientation()->getPosition());
		MINEobj->setRotation(players[0]->getOrientation()->getRotationAngle());
		MINEobj->AttachObject(MINE_OBJ);
		MINEZ.push_back(MINEobj);
		MINETIMER.push_back(MAX_MINE_TIMER);
		MINEobj->updateTransforms(DT);
		//std::cout << "TRIGGER1" << std::endl;
		currentCoolDown = coolDownExp;
	}
	if (currentCoolDown >= 0.f)
		currentCoolDown -= DT;

	for (int i = MINEZ.size() -1; i >= 0; i--)
	{
		MINETIMER[i] -= DT;
		if (MINETIMER[i] <= 0.f)
		{
			MINETIMER.erase(MINETIMER.begin() + i);
			EXPLOSIONS.push_back(MINEZ[i]);
			MINEZ[i]->RemoveObject(0);
			MINEZ[i]->AttachObject(EXP_OBJ);
			MINEZ[i]->updateTransforms(DT);
			MINEZ.erase(MINEZ.begin() + i);
			SPLODETIMER.push_back(MAX_EXP_TIMER);
			//std::cout << "TRIGGER2" << std::endl;
		}
	}

	for (int i = EXPLOSIONS.size() - 1; i >= 0; i--)
	{
		SPLODETIMER[i] -= DT;
		if (SPLODETIMER[i] <= 0.f)
		{
			SPLODETIMER.erase(SPLODETIMER.begin() + i);
			EXPLOSIONS[i]->RemoveObject(0);
			delete EXPLOSIONS[i];
			EXPLOSIONS.erase(EXPLOSIONS.begin() + i);
			//std::cout << "TRIGGER3" << std::endl;
		}
	}

	for (int i = 0; i < SHAKEYOBJ.size(); i++)
	{
		for (int j = SHAKEYBUFFER.size() - 1; j >= 0; j--)
		{
			if (SHAKEYOBJ[i] == SHAKEYBUFFER[j])
				SHAKEYBUFFER.erase(SHAKEYBUFFER.begin() + j);
		}
	}

	for (int j = 0; j < SHAKEYBUFFER.size(); j++)
		SHAKEYOBJ.push_back(SHAKEYBUFFER[j]);

	for (int i = SHAKEYOBJ.size() - 1; i >= 0; i--)
	{
		SHAKEYOBJ[i]->updateBezShake(DT);
		if (SHAKEYOBJ[i]->getTimeOnBez() <= 0.0001f)
			SHAKEYOBJ.erase(SHAKEYOBJ.begin() + i);
	}

	for (int i = 0; i < dynamicCollisions.size(); i++)
	{
		bool areYa = false;
		//int numOf = 0;
		for (int j = 0; j < theMap->getSection(pcX, pcY)->getNumObjOnFace(); j++)
		{
			Object* OOO = theMap->getSection(pcX, pcY)->getObjectOnFace(j);
			if (dealWithCol(dynamicCollisions[i], OOO))
			{
				areYa = true;
				//numOf++;
			}
		}
		for (int k = 0; k < 8; k++)
		{
			if (theMap->getSection(pcX, pcY)->getCompact()->at(k) != nullptr)
			{
				for (int j = 0; j < theMap->getSection(pcX, pcY)->getCompact()->at(k)->getNumObjOnFace(); j++)
				{
					Object* OOO = theMap->getSection(pcX, pcY)->getCompact()->at(k)->getObjectOnFace(j);
					if (dealWithCol(dynamicCollisions[i], OOO))
					{
						areYa = true;
						//numOf++;
					}
				}
			}
		}
		//if (dynamicCollisions[i]->getOrientation()->getPosition().x < 12.f)
		//{
		//	//std::cout << LEFT_WALL->getOrientation()->getPosition().x << std::endl;
		//	if (dealWithCol(dynamicCollisions[i], LEFT_WALL))
		//		areYa = true;
		//}
		//if (dynamicCollisions[i]->getOrientation()->getPosition().z < 12.f)
		//{
		//	if (dealWithCol(dynamicCollisions[i], TOP_WALL))
		//		areYa = true;
		//}
		//if (dynamicCollisions[i]->getOrientation()->getPosition().x > 582.f)
		//{
		//	if (dealWithCol(dynamicCollisions[i], RIGHT_WALL))
		//		areYa = true;
		//}
		//if (dynamicCollisions[i]->getOrientation()->getPosition().z > 582.f)
		//{
		//	if (dealWithCol(dynamicCollisions[i], BOTTOM_WALL))
		//		areYa = true;
		//}
		//std::cout << dynamicCollisions[i]->getPhysicsBody()->velocity.Length() << std::endl;
		//if (numOf > 1)
		//	dynamicCollisions[i]->getPhysicsBody()->position = dynamicCollisions[i]->getPhysicsBody()->prePos;
		if (areYa)
			dynamicCollisions[i]->getPhysicsBody()->velocity *= -0.2f;
		//std::cout << "----------" << std::endl;
	}

	//...
		//vec4 MPU = camera.getView().GetTranspose() * camera.getProjection().GetTranspose().GetInverse() * vec4(2.f * mousePosition.x / WINDOW_WIDTH - 1.f, 2.f * mousePosition.y / WINDOW_HEIGHT - 1.f, 0, 1) + vec4(camera.getPosition(), 0);

	//std::cout << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << ", " << std::endl;

	mouseHandler();
	keyHandler();
}

void Game::draw()
{
	toDraw.clear();
	///clear buffers///
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	FB.Clear();
	WorkBuffer1.Clear();
	WorkBuffer2.Clear();
	testBuff.Clear();
	UISCREEN.Clear();

	///Viewport///
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	//glUseProgram(program);
	//
	//int mvpLoc = glGetUniformLocation(program, "u_mvp");
	//
	//mat4 mvp = camera.getProjection() * camera.getLocalToWorldMatrix().GetInverse() * rot1.getLocalToWorldMatrix();
	//glUniformMatrix4fv(mvpLoc, 1, false, mvp.data);

	///render the scene///

	lightsToDraw.push_back(SUN);
	lightMats.push_back(mat4::Identity);

	PassThrough.Bind();

	PassThrough.SendUniform("uTex", 0);
	PassThrough.SendUniform("DT", TotalGameTime);

	vec2 camPosUnit = vec2(camera.getPosition().x, camera.getPosition().z) / tileSize;

	vec2 CPOSU = vec2(players[0]->getOrientation()->getPosition().x, players[0]->getOrientation()->getPosition().z) / tileSize;
	//FB.Bind();

	//texture->bind();
	PassThrough.SendUniformMat4("uView", camera.getLocalToWorldMatrix().GetInverse().GetTranspose().data, false);
	PassThrough.SendUniformMat4("uProj", camera.getProjection().GetTranspose().data, false);

	for (int i = -4 + CPOSU.y; i < 5 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = -4 + CPOSU.x; j < 5 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				PassThrough.SendUniform("YEET", 0.f);
				drawObjectMesh(theMap->getSection(j, i)->getPlane(), mat4::Identity, &PassThrough, vec3(0, 0, 0), vec3(0, 0, 0), 0.f);
				for (int k = 0; k < theMap->getSection(j, i)->getNumObjOnFace(); k++)
				{
					PassThrough.SendUniform("YEET", theMap->getSection(j, i)->getObjectOnFace(k)->YEET);
					drawObjectMesh(theMap->getSection(j, i)->getObjectOnFace(k), mat4::Identity, &PassThrough, vec3(0, 0, 0), vec3(0, 0, 0), 0.f);
				}
			}
		}
	}

	//for (int i = 0; i < players[0]->getChild(0)->getNumberOfChildren(); i++)
	//{
	//	std::cout << players[0]->getChild(0)->getChild(i)->getOrientation()->getPosition().x << ", "
	//		<< players[0]->getChild(0)->getChild(i)->getOrientation()->getPosition().y << ", "
	//		<< players[0]->getChild(0)->getChild(i)->getOrientation()->getPosition().z << ", "
	//		<< std::endl;
	//}
	//std::cout << "---------" << std::endl;

	for (int i = 0; i < 24 && i < lightsToDraw.size(); i++)
	{
		PassThrough.SendUniform("position[" + std::to_string(i) + "]", lightMats[i] * lightsToDraw[i]->getPosition());
		PassThrough.SendUniform("direction[" + std::to_string(i) + "]", lightMats[i] * lightsToDraw[i]->getDirection());
		PassThrough.SendUniform("DSE[" + std::to_string(i) + "]", lightsToDraw[i]->getDiffSpecExp());
		PassThrough.SendUniform("CLQ[" + std::to_string(i) + "]", lightsToDraw[i]->getCLQ());
		PassThrough.SendUniform("color[" + std::to_string(i) + "]", lightsToDraw[i]->getColor());
		PassThrough.SendUniform("lightType[" + std::to_string(i) + "]", lightsToDraw[i]->getLightType());
		//std::cout << lightsToDraw.size() << std::endl;
		//vec4 lPos = lightsToDraw[i]->getToWorld() * lightsToDraw[i]->getPosition();
		//std::cout << lPos.x << ", " << lPos.y << " , " << lPos.z << std::endl;
	}
	//vec3 pPos = players[0]->getOrientation()->getPosition();
	//std::cout << pPos.x << ", " << pPos.y << " , " << pPos.z << std::endl;
	int num = lightsToDraw.size();
	if (num > 24)
		num = 24;

	PassThrough.SendUniform("ambient", ambientLevel);
	PassThrough.SendUniform("Lsize", num);

	lightsToDraw.clear();
	lightMats.clear();

	MINESHADER.Bind();
	MINESHADER.SendUniform("uTex", 0);

	MINESHADER.SendUniformMat4("uView", camera.getLocalToWorldMatrix().GetInverse().GetTranspose().data, false);
	MINESHADER.SendUniformMat4("uProj", camera.getProjection().GetTranspose().data, false);

	for (int i = 0; i < MINEZ.size(); i++)
	{
		drawMines(MINEZ[i], mat4::Identity, &MINESHADER);
	}

	EXPLOSIONSHADER.Bind();
	EXPLOSIONSHADER.SendUniform("uTex", 0);
	EXPLOSIONSHADER.SendUniform("uR", 1);
	EXPLOSIONSHADER.SendUniform("uG", 2);

	EXPLOSIONSHADER.SendUniformMat4("uView", camera.getLocalToWorldMatrix().GetInverse().GetTranspose().data, false);
	EXPLOSIONSHADER.SendUniformMat4("uProj", camera.getProjection().GetTranspose().data, false);

	for (int i = 0; i < EXPLOSIONS.size(); i++)
	{
		EXPLOSIONSHADER.SendUniform("TimeRem", SPLODETIMER[i] / MAX_EXP_TIMER);
		drawExplosions(EXPLOSIONS[i], mat4::Identity, &EXPLOSIONSHADER);
	}

	//FB.Unbind();
	glBindVertexArray(GL_NONE);


	ShaderProgram::Unbind();

	///compute the High Pass///
	//glViewport(0, 0, WINDOW_WIDTH / BLOOM_DOWNSCALE, WINDOW_HEIGHT / BLOOM_DOWNSCALE);
	//
	//BloomHP.Bind();
	//BloomHP.SendUniform("uTex", 0);
	//BloomHP.SendUniform("uThreshold", BLOOM_THRESHOLD);
	//
	//addPostProcessLink(&WorkBuffer1, &FB, true);
	//ShaderProgram::Unbind();
	//
	/////compute blur///
	//
	//for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
	//{
	//	BloomHB.Bind();
	//	BloomHB.SendUniform("uTex", 0);
	//	BloomHB.SendUniform("uPixelSize", 1.0f / WINDOW_WIDTH);
	//	
	//	addPostProcessLink(&WorkBuffer2, &WorkBuffer1, true);
	//	BloomHB.Unbind();
	//
	//	BloomVB.Bind();
	//	BloomVB.SendUniform("uTex", 0);
	//	BloomVB.SendUniform("uPixelSize", 1.0f / WINDOW_HEIGHT);
	//
	//	addPostProcessLink(&WorkBuffer1, &WorkBuffer2, true);
	//	BloomVB.Unbind();
	//}
	//
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//
	//BloomCOMP.Bind();
	//BloomCOMP.SendUniform("uScene", 0);
	//BloomCOMP.SendUniform("uBloom", 1);
	//
	//std::vector<FrameBuffer*> tempHolder;
	//tempHolder.push_back(&FB);
	//tempHolder.push_back(&WorkBuffer1);
	//std::vector<bool> actives;
	//actives.push_back(normalRenderActive);
	//actives.push_back(bloomActive);
	//addPostProcessLink(&testBuff, tempHolder, actives);
	//
	//BloomCOMP.Unbind();
	//
	//testBuff.MovetoBackBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	//
	////Moving data to back buffer at the same time as our last post process
	//GreyScalePost.Bind();
	//GreyScalePost.SendUniform("uTex", 0);
	//GreyScalePost.SendUniform("totTime", TotalGameTime);
	//GreyScalePost.SendUniform("WW", (float) WINDOW_WIDTH);
	//GreyScalePost.SendUniform("WH", (float) WINDOW_HEIGHT);
	//
	//addPostProcessLink(nullptr, &testBuff, rippleActive);
	//
	//GreyScalePost.Unbind();

	glDisable(GL_DEPTH_TEST);

	//UI STUFF GOES HERE!

	UI_SHADER.Bind();

	UI_SHADER.SendUniformMat4("uView", UIcam.getLocalToWorldMatrix().GetInverse().GetTranspose().data, false);
	UI_SHADER.SendUniformMat4("uProj", UIcam.getProjection().GetTranspose().data, false);

	for (int i = 0; i < UIELEM.size(); i++)
	{
		drawUIObject(UIELEM[i], mat4::Identity, &UI_SHADER);
	}

	glBindVertexArray(GL_NONE);
	ShaderProgram::Unbind();

	glEnable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	//switch(key)
	//{
	//case 32: // the space bar
	//	break;
	//case 'd':
	//	break;
	//case 27: // the escape key
	//case 'q': // the 'q' key
	//	exit(1);
	//	break;
	//}
	setKeysDown(true, key);
	//std::cout << key << std::endl;
	if (key == KEY_ESC)
	{
		exit(1);
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	//switch(key)
	//{
	//case 32: // the space bar
	//	if (isProj)
	//	{
	//		camera.orthographic(-10.f * aspectRatio, 10.f * aspectRatio, 10.f, -10.f, -1.f, 1000.f);
	//		isProj = false;
	//	}
	//	else
	//	{
	//		camera.perspective(60.0f, aspectRatio, 1.0f, 1000.f);
	//		isProj = true;
	//	}
	//	break;
	//case 27: // the escape key
	//case 'q': // the 'q' key
	//	exit(1);
	//	break;
	//}
	setKeysDown(false, key);
}

void Game::setKeysDown(bool down, unsigned char key)
{
	keysDown[key - 1] = down;
}

void Game::keyHandler()
{
	for (int i = 0; i < 256; i++)
		backCheckKeysDown[i] = keysDown[i];
}

void Game::mouseHandler()
{
	for (int i = 0; i < 10; i++)
		backCheckMouseDown[i] = mouseDown[i];
}

void Game::triggerHandler()
{
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	//if(state == GLUT_DOWN) 
	//{
	//	switch(button)
	//	{
	//	case GLUT_LEFT_BUTTON:
	//		std::cout << "LLLLLLLLEFT!" << std::endl;
	//		break;
	//	case GLUT_RIGHT_BUTTON:
	//		std::cout << "RRRRRRRIGHT!" << std::endl;
	//		break;
	//	case GLUT_MIDDLE_BUTTON:
	//		std::cout << "IIIIIIIIIMPOSSIBLE WITH A TRACKPAD!" << std::endl;
	//		break;
	//	}
	//}
	//else
	//{
	//	switch (button)
	//	{
	//	case GLUT_LEFT_BUTTON:
	//		std::cout << "RELEASE PORT!" << std::endl;
	//		break;
	//	case GLUT_RIGHT_BUTTON:
	//		std::cout << "RELEASE STARBOARD!" << std::endl;
	//		break;
	//	case GLUT_MIDDLE_BUTTON:
	//		std::cout << "RELEASE THE KRAKEN!" << std::endl;
	//		break;
	//	}
	//}

	/*
	if (state == GLUT_DOWN)
	{
		float TotDur = 1.0f;
		float duratioN = TotDur;

		placeAndTime.push_back(vec4(0, 0, TotDur, duratioN));
	}
	else
	{

	}
	*/

	if (state == GLUT_DOWN)
		mouseDown[button] = true;
	else
		mouseDown[button] = false;
	//std::cout << mouseDown[3] << std::endl;
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
	//std::cout << x + ", " + y << std::endl;
	//camera.setRotationAngleY(camera.getRotationAngleY() + 0.01f * x);
	mousePosition.x = x;
	mousePosition.y = y;
}

void Game::addPostProcessLink(FrameBuffer * FB, std::vector<FrameBuffer*> PrevTexArray, std::vector<bool> allowEffect)
{
	bool anyEffect = false;
	if (FB != nullptr)
		FB->Bind();
	for (int i = 0; i < PrevTexArray.size(); i++)
	{
		if (allowEffect[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, PrevTexArray[i]->GetColorHandle(0));
			anyEffect = true;
		}
	}
	if (anyEffect)
		drawFullscreenQuad();
	for (int i = PrevTexArray.size() - 1; i >= 0; i--)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
	if (FB != nullptr)
		FB->Unbind();
}

void Game::addPostProcessLink(FrameBuffer * FB, FrameBuffer * PrevTexArray, bool allowEffect)
{
	if (FB != nullptr)
		FB->Bind();
	glBindTexture(GL_TEXTURE_2D, PrevTexArray->GetColorHandle(0));
	if (allowEffect)
		drawFullscreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	if (FB != nullptr)
		FB->Unbind();
}

bool Game::dealWithCol(Object * O1, Object * O2)
{
	if (O1 != O2 && O1->getChild(0)->getPhysicsBody()->collider != nullptr && O2->getChild(0)->getPhysicsBody()->collider != nullptr && !O2->isDestroyed() && !O1->isDestroyed())
	{
		if (O1->getChild(0)->getPhysicsBody()->collider->collidesWith(
			O2->getChild(0)->getPhysicsBody()->collider,
			&O1->getOrientation()->getLocalToWorldMatrix(),
			&O2->getOrientation()->getLocalToWorldMatrix()))
		{
			//vec4 sub = O2->getOrientation()->getLocalToWorldMatrix() * vec4(0, 0, 0, 1);
			//std::cout << sub.x <<", " << sub.y <<", " << sub.z << std::endl;
			float velRat = O1->getPhysicsBody()->velocity.Length() / O1->getPhysicsBody()->velocityLimit;
			if (O2->getChild(0)->getPhysicsBody()->collider->grass)
			{
				vec3 pressDownGrass = (-O1->getChild(0)->getPhysicsBody()->collider->outDir * 12.f + vec3(0, -1.f, 0)) * velRat;
				O2->setBezPointOut(pressDownGrass, pressDownGrass);
				O2->setBezTimeToHalf();
				SHAKEYBUFFER.push_back(O2);
				return false;
			}
			float contAng = Dot(O1->getPhysicsBody()->velocity.GetNormalized(), O1->getChild(0)->getPhysicsBody()->collider->outDir);
			if (contAng < 0)
			{
				//dynamicCollisions[i]->getPhysicsBody()->position -= contAng * 1.0001f * dynamicCollisions[i]->getPhysicsBody()->velocity.Length() * dynamicCollisions[i]->getChild(0)->getPhysicsBody()->collider->outDir * DT;
				//areYa = true;
				O1->getPhysicsBody()->position -= contAng * 1.0001f * O1->getPhysicsBody()->velocity.Length() * O1->getChild(0)->getPhysicsBody()->collider->outDir * DT;
				//std::cout << O1->getChild(0)->getPhysicsBody()->collider->outDir.x << " ----- " << O1->getChild(0)->getPhysicsBody()->collider->outDir.z << std::endl;
				//O1->getPhysicsBody()->velocity *= contAng * 0.2f;
				//if (contAng > -1.f)
				//{
				//	vec3 YEROUT = (O1->getPhysicsBody()->velocity - O1->getChild(0)->getPhysicsBody()->collider->outDir * 
				//		Dot(O1->getChild(0)->getPhysicsBody()->collider->outDir, O1->getPhysicsBody()->velocity)).GetNormalized() * O1->getPhysicsBody()->velocity.Length();
				//	float vom = O1->getPhysicsBody()->velocity.Length() / O1->getPhysicsBody()->velocityLimit;
				//	float maxSteer = 5000.f;
				//
				//	if (ToDegrees(acos(Dot(YEROUT, getPhysicsBody()->direction.GetNormalized()))) > maxSteer * vom * vom * DT)
				//	{
				//
				//	}
				//}
				//O1->getPhysicsBody()->velocity *= contAng;

				if (velRat > 0.4f)
				{
					O2->setBezPointOut(O1->getPhysicsBody()->velocity.GetNormalized() * velRat,
						(-O1->getPhysicsBody()->velocity + 2.f * Dot(O1->getPhysicsBody()->velocity, O1->getChild(0)->getPhysicsBody()->collider->outDir)
							* O1->getChild(0)->getPhysicsBody()->collider->outDir).GetNormalized() * velRat);
					O2->resetTimeOnBez();
					SHAKEYOBJ.push_back(O2);
					//std::cout << "OOF! " << O2->getTimeOnBez() << std::endl;
				}

				return true;
			}
			//dynamicCollisions[i]->getPhysicsBody()->vDir *= -1.f;
		}
	}
	return false;
}

bool Game::dealWithExplosions(Object * EXP, Object * DESTR)
{
	if (EXP->getChild(0)->getPhysicsBody()->collider != nullptr && DESTR->getChild(0)->getPhysicsBody()->collider != nullptr && !DESTR->isDestroyed())
	{
		if (EXP->getChild(0)->getPhysicsBody()->collider->collidesWith(
			DESTR->getChild(0)->getPhysicsBody()->collider,
			&EXP->getOrientation()->getLocalToWorldMatrix(),
			&DESTR->getOrientation()->getLocalToWorldMatrix()))
		{
			bool doubleCheck = true;
			for (int i = 0; i < GONE.size(); i++)
				if (GONE[i] == DESTR)
				{
					doubleCheck = false;
					i = GONE.size();
				}
			if (doubleCheck)
				GONE.push_back(DESTR);
			return true;
		}
	}
	return false;
}

bool Game::dealWithDestruction(Object * defender, Object * offender)
{
	return false;
}

void Game::drawMines(Object * _OBJ, mat4 previous, ShaderProgram * SP)
{
	mat4 newMat = previous * _OBJ->getOrientation()->getLocalToWorldMatrix();

	if (_OBJ->visible())
	{
		if (_OBJ->getMesh() != nullptr)
		{
			SP->SendUniformMat4("uModel", newMat.GetTranspose().data, false);

			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->bind();
			glBindVertexArray(_OBJ->getMesh()->VAO);

			glDrawArrays(GL_TRIANGLES, 0, _OBJ->getMesh()->GetNumVertices());
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->unbind();
		}
	}

	for (int i = 0; i < _OBJ->getNumberOfChildren(); i++)
	{
		drawUIObject(_OBJ->getChild(i), newMat, SP);
	}
}

void Game::drawExplosions(Object * _OBJ, mat4 previous, ShaderProgram * SP)
{
	mat4 newMat = previous * _OBJ->getOrientation()->getLocalToWorldMatrix();

	if (_OBJ->visible())
	{
		if (_OBJ->getMesh() != nullptr)
		{
			SP->SendUniformMat4("uModel", newMat.GetTranspose().data, false);

			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->bind(0);
			if (_OBJ->getTexture(1) != nullptr)
				_OBJ->getTexture(1)->getTexture()->bind(1);
			if (_OBJ->getTexture(2) != nullptr)
				_OBJ->getTexture(2)->getTexture()->bind(2);
			glBindVertexArray(_OBJ->getMesh()->VAO);

			glDrawArrays(GL_TRIANGLES, 0, _OBJ->getMesh()->GetNumVertices());
			if (_OBJ->getTexture(2) != nullptr)
				_OBJ->getTexture(2)->getTexture()->unbind(2);
			if (_OBJ->getTexture(1) != nullptr)
				_OBJ->getTexture(1)->getTexture()->unbind(1);
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->unbind(0);
		}
	}

	for (int i = 0; i < _OBJ->getNumberOfChildren(); i++)
	{
		drawExplosions(_OBJ->getChild(i), newMat, SP);
	}
}

void Game::drawObjectMesh(Object* _OBJ, mat4 previous, ShaderProgram *SP, vec3 B1P, vec3 B2P, float tT)
{
	mat4 newMat = previous * _OBJ->getOrientation()->getLocalToWorldMatrix();

	vec3 hold1 = B1P;
	vec3 hold2 = B2P;
	float holdT = tT;
	if (_OBJ->getTimeOnBez() > 0)
	{
		hold1 = _OBJ->getB1();
		hold2 = _OBJ->getB2();
		holdT = _OBJ->getTimeOnBez();

		//std::cout << hold1.x << ", " << hold1.y << ", " << hold1.z << ", " << std::endl;
		//std::cout << hold2.x << ", " << hold2.y << ", " << hold2.z << ", " << std::endl;
	}

	if (_OBJ->visible())
	{
		if (_OBJ->getMesh() != nullptr)
		{
			SP->SendUniform("BEZTIME", holdT * holdT);
			SP->SendUniform("BP1", hold1);
			SP->SendUniform("BP2", hold2);
			SP->SendUniformMat4("uModel", newMat.GetTranspose().data, false);
			
			//if (holdT > 0)
			//{
			//	std::cout << hold1.x << ", " << hold1.y << ", " << hold1.z << ", " << std::endl;
			//	std::cout << hold2.x << ", " << hold2.y << ", " << hold2.z << ", " << std::endl;
			//	std::cout << holdT << std::endl;
			//}

			//std::cout << newMat.GetTranslation().x << ", " << newMat.GetTranslation().y << ", " << newMat.GetTranslation().z << std::endl;
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->bind();
			glBindVertexArray(_OBJ->getMesh()->VAO);
			
			glDrawArrays(GL_TRIANGLES, 0, _OBJ->getMesh()->GetNumVertices());
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->unbind();
		}
		if (_OBJ->getLight() != nullptr)
		{
			lightMats.push_back(newMat);
			lightsToDraw.push_back(_OBJ->getLight());
			//std::cout << "WE MADE IT THIS FAR!" << std::endl;
		}
		for (int i = 0; i < _OBJ->getNumberOfChildren(); i++)
		{
			drawObjectMesh(_OBJ->getChild(i), newMat, SP, hold1, hold2, holdT);
		}
	}
}

void Game::drawUIObject(Object * _OBJ, mat4 previous, ShaderProgram *SP)
{
	mat4 newMat = previous * _OBJ->getOrientation()->getLocalToWorldMatrix();

	if (_OBJ->visible())
	{
		if (_OBJ->getMesh() != nullptr)
		{
			SP->SendUniformMat4("uModel", newMat.GetTranspose().data, false);

			//std::cout << newMat.GetTranslation().x << ", " << newMat.GetTranslation().y << ", " << newMat.GetTranslation().z << std::endl;
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->bind();
			glBindVertexArray(_OBJ->getMesh()->VAO);
			glDrawArrays(GL_TRIANGLES, 0, _OBJ->getMesh()->GetNumVertices());
			if (_OBJ->getTexture(0) != nullptr)
				_OBJ->getTexture(0)->getTexture()->unbind();
		}
		for (int i = 0; i < _OBJ->getNumberOfChildren(); i++)
		{
			drawUIObject(_OBJ->getChild(i), newMat, SP);
		}
	}
}

void Game::drawOnlyPlayer(Object * _OBJ, mat4 previous, ShaderProgram * SP)
{

}

void Game::loadAllTextures(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Textures/" + fileName + ".txt");
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
			TextureLayer* texLayer = new TextureLayer;
			texLayer->setTexture(tex);
			//std::cout << temp << std::endl;
			texLayer->setTextureName(temp);
			textures.push_back(texLayer);
		}
	}
	masterFile.close();
}

void Game::loadAllMeshes(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Models/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Mesh* m = new Mesh;
		if (!m->LoadFromFile("Assets/Models/" + temp + ".obj"))
		{
			std::cout << temp << " model could not load!" << std::endl;
			delete m;
			m = nullptr;
		}
		else
		{
			m->ID = temp;
			//std::cout << temp << std::endl;
			meshList.push_back(m);
		}
	}
	masterFile.close();
}

void Game::loadAllObjects(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Objects/" + fileName + ".txt");
	std::ifstream objectFiles;
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Object* obj = new Object;
		objectFiles.open("Assets/Objects/" + temp + ".txt");
		std::string meshName, textureName, BRSTR;
		std::string dataparse;
		float boundRadius;
		std::getline(objectFiles, meshName);
		std::getline(objectFiles, textureName);
		std::getline(objectFiles, BRSTR);
		boundRadius = std::stof(BRSTR);
		bool hasTex = false, hasMesh = false;
		if (textureName == "EMPTY")
		{
			obj->addTexture(nullptr);
			hasTex = true;
		}
		else
		{
			for (int i = 0; i < textures.size(); i++)
			{
				if (textures[i]->getTextureName() == textureName)
				{
					obj->addTexture(textures[i]);
					i = textures.size();
					hasTex = true;
				}
			}
		}
		if (meshName == "EMPTY")
		{
			obj->AttachMesh(nullptr);
			hasMesh = true;
		}
		else
		{
			for (int i = 0; i < meshList.size(); i++)
			{
				if (meshList[i]->ID == meshName)
				{
					obj->AttachMesh(meshList[i]);
					i = meshList.size();
					hasMesh = true;
				}
			}
		}
		obj->setBoundingRadius(boundRadius);
		if (!hasTex || !hasMesh)
		{
			std::cout << temp << " game object could not load!" << std::endl;
			if (!hasTex)
				std::cout << "Texture not found!" << std::endl;
			if (!hasMesh)
				std::cout << "Mesh not found!" << std::endl;
			delete obj;
			obj = nullptr;
		}
		else
		{
			if (obj->getMesh() != nullptr)
				if (obj->getMesh()->ID == "TESTING_BASE_PLATE")
					obj->setIsBasePlate(true);
			obj->setIdentity(temp);
			objects.push_back(obj);
			while (std::getline(objectFiles, dataparse))
			{
				if (dataparse == "OBJECT")
				{
					std::getline(objectFiles, dataparse);
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

					Transform TF;
					TF.setPosition(nPos);
					TF.setRotationAngle(nRot);
					TF.setScale(nScale);
					TF.update(DT);

					createChild(dataparse, obj, TF);
				}
				else if (dataparse == "LIGHT")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < allLight.size(); i++)
					{
						if (dataparse == allLight[i]->getName())
						{
							obj->setLight(allLight[i]);
						}
					}
				}
				else if (dataparse == "PHYS")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < allHitboxes.size(); i++)
					{
						if (dataparse == allHitboxes[i]->name)
						{
							PhysicsBody PB(allHitboxes[i]);
							obj->setPhysicsBody(PB);
							//std::cout << "FOUND!" << std::endl;
						}
					}
					std::getline(objectFiles, dataparse);
					if (dataparse == "true")
						obj->getPhysicsBody()->collider->dynamic = true;
					else
						obj->getPhysicsBody()->collider->dynamic = false;
					if (dataparse == "grass")
						obj->getPhysicsBody()->collider->grass = true;
				}
				else if (dataparse == "TARGET")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < meshList.size(); i++)
					{
						if (dataparse == meshList[i]->ID)
						{
							obj->addTarget(meshList[i]);
						}
					}
				}
				else if (dataparse == "TEXTURE")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < textures.size(); i++)
					{
						if (dataparse == textures[i]->getTextureName())
						{
							obj->addTexture(textures[i]);
						}
					}
				}
			}
		}
		objectFiles.close();
	}
	masterFile.close();

	std::string fetch = "_PLAYER_MASTER_LIST";
	loadAllPlayerObjects(fetch);

	//TODO - make it so that a certain placeholder object is replaced by a player!
	//TODO - make it so that the camera follows the player!
}

void Game::loadAllUIElements(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/UI/" + fileName + ".txt");
	std::ifstream objectFiles;
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Object* obj = new Object;
		objectFiles.open("Assets/UI/" + temp + ".txt");
		std::string meshName, textureName;
		std::getline(objectFiles, meshName);
		std::getline(objectFiles, textureName);
		bool hasTex = false, hasMesh = false;
		for (int i = 0; i < textures.size(); i++)
		{
			if (textures[i]->getTextureName() == textureName)
			{
				obj->addTexture(textures[i]);
				i = textures.size();
				hasTex = true;
			}
		}
		for (int i = 0; i < meshList.size(); i++)
		{
			if (meshList[i]->ID == meshName)
			{
				obj->AttachMesh(meshList[i]);
				i = meshList.size();
				hasMesh = true;
			}
		}
		if (!hasTex || !hasMesh)
		{
			std::cout << temp << " UI element could not load!" << std::endl;
			if (!hasTex)
				std::cout << "Texture not found!" << std::endl;
			if (!hasMesh)
				std::cout << "Mesh not found!" << std::endl;
			delete obj;
			obj = nullptr;
		}
		else
		{
			if (obj->getMesh()->ID == "TESTING_BASE_PLATE")
				obj->setIsBasePlate(true);
			obj->setIdentity(temp);
			UIELEM.push_back(obj);
		}
		objectFiles.close();
	}
	masterFile.close();
}

void Game::loadAllLights(std::string & filename)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Lights/" + filename + ".txt");
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
	unsigned int lightType = 0;
	while (std::getline(masterFile, temp))
	{
		lightFiles.open("Assets/Lights/" + temp + ".txt");
		std::getline(lightFiles, dataHolder);
		if (dataHolder == "DIRECTIONAL")
			lightType = 0;
		else if (dataHolder == "POINT")
			lightType = 1;
		else if (dataHolder == "SPOT")
			lightType = 2;

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
		clq.w = std::stof(dataHolder);

		Light* lot = new Light(dir, pos, dse, clq, col, temp, lightType);
		allLight.push_back(lot);
		lightFiles.close();
	}
	masterFile.close();
}

void Game::loadAllHitboxes(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Hitboxes/" + fileName + ".txt");
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Hitbox2D* HB = new Hitbox2D;
		HB->name = temp;
		HB->loadFromFile("Assets/Hitboxes/" + temp + ".obj");
		allHitboxes.push_back(HB);
		//std::cout << temp << std::endl;
	}
	masterFile.close();
}

void Game::loadAllPlayerObjects(std::string & fileName)
{
	std::ifstream masterFile;
	masterFile.open("Assets/Objects/" + fileName + ".txt");
	std::ifstream objectFiles;
	std::string temp;
	while (std::getline(masterFile, temp))
	{
		Object* obj = new Object;
		objectFiles.open("Assets/Objects/" + temp + ".txt");
		std::string meshName, textureName, BRSTR;
		std::string dataparse;
		float boundRadius;
		std::getline(objectFiles, meshName);
		std::getline(objectFiles, textureName);
		std::getline(objectFiles, BRSTR);
		boundRadius = std::stof(BRSTR);
		bool hasTex = false, hasMesh = false;
		if (textureName == "EMPTY")
		{
			obj->addTexture(nullptr);
			hasTex = true;
		}
		else
		{
			for (int i = 0; i < textures.size(); i++)
			{
				if (textures[i]->getTextureName() == textureName)
				{
					obj->addTexture(textures[i]);
					i = textures.size();
					hasTex = true;
				}
			}
		}
		if (meshName == "EMPTY")
		{
			obj->AttachMesh(nullptr);
			hasMesh = true;
		}
		else
		{
			for (int i = 0; i < meshList.size(); i++)
			{
				if (meshList[i]->ID == meshName)
				{
					obj->AttachMesh(meshList[i]);
					i = meshList.size();
					hasMesh = true;
				}
			}
		}
		obj->setBoundingRadius(boundRadius);
		if (!hasTex || !hasMesh)
		{
			std::cout << temp << " game object could not load!" << std::endl;
			if (!hasTex)
				std::cout << "Texture not found!" << std::endl;
			if (!hasMesh)
				std::cout << "Mesh not found!" << std::endl;
			delete obj;
			obj = nullptr;
		}
		else
		{
			if (obj->getMesh() != nullptr)
				if (obj->getMesh()->ID == "TESTING_BASE_PLATE")
					obj->setIsBasePlate(true);
			obj->setIdentity(temp);
			objects.push_back(obj);
			while (std::getline(objectFiles, dataparse))
			{
				if (dataparse == "OBJECT")
				{
					std::getline(objectFiles, dataparse);
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

					Transform TF;
					TF.setPosition(nPos);
					TF.setRotationAngle(nRot);
					TF.setScale(nScale);
					TF.update(DT);

					createChild(dataparse, obj, TF);
				}
				else if (dataparse == "LIGHT")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < allLight.size(); i++)
					{
						if (dataparse == allLight[i]->getName())
						{
							obj->setLight(allLight[i]);
						}
					}
				}
				else if (dataparse == "PHYS")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < allHitboxes.size(); i++)
					{
						if (dataparse == allHitboxes[i]->name)
						{
							PhysicsBody PB(allHitboxes[i]);
							obj->setPhysicsBody(PB);
						}
					}
					std::getline(objectFiles, dataparse);
					if (dataparse == "true")
						obj->getPhysicsBody()->collider->dynamic = true;
					else
						obj->getPhysicsBody()->collider->dynamic = false;
				}
				else if (dataparse == "TARGET")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < meshList.size(); i++)
					{
						if (dataparse == meshList[i]->ID)
						{
							obj->addTarget(meshList[i]);
						}
					}
				}
				else if (dataparse == "TEXTURE")
				{
					std::getline(objectFiles, dataparse);
					for (int i = 0; i < textures.size(); i++)
					{
						if (dataparse == textures[i]->getTextureName())
						{
							obj->addTexture(textures[i]);
							//std::cout << "LODED" << std::endl;
						}
					}
				}
			}
		}
		objectFiles.close();
	}
	masterFile.close();
}

void Game::loadAllMorphTargs(std::string & fileName)
{
	
}

void Game::createChild(std::string & fileName, Object * parent, Transform TF)
{
	std::ifstream objectFiles;
	Object* obj = new Object;
	objectFiles.open("Assets/Objects/" + fileName + ".txt");
	std::string meshName, textureName, BRSTR;
	std::string dataparse;
	float boundRadius;
	std::getline(objectFiles, meshName);
	std::getline(objectFiles, textureName);
	std::getline(objectFiles, BRSTR);
	boundRadius = std::stof(BRSTR);
	bool hasTex = false, hasMesh = false;
	if (textureName == "EMPTY")
	{
		obj->addTexture(nullptr);
		hasTex = true;
	}
	else
	{
		for (int i = 0; i < textures.size(); i++)
		{
			if (textures[i]->getTextureName() == textureName)
			{
				obj->addTexture(textures[i]);
				i = textures.size();
				hasTex = true;
			}
		}
	}
	if (meshName == "EMPTY")
	{
		obj->AttachMesh(nullptr);
		hasMesh = true;
	}
	else
	{
		for (int i = 0; i < meshList.size(); i++)
		{
			if (meshList[i]->ID == meshName)
			{
				obj->AttachMesh(meshList[i]);
				i = meshList.size();
				hasMesh = true;
			}
		}
	}
	obj->setBoundingRadius(boundRadius);
	if (!hasTex || !hasMesh)
	{
		std::cout << fileName << " game object could not load!" << std::endl;
		if (!hasTex)
			std::cout << "Texture not found!" << std::endl;
		if (!hasMesh)
			std::cout << "Mesh not found!" << std::endl;
		delete obj;
		obj = nullptr;
	}
	else
	{
		if (obj->getMesh() != nullptr)
			if (obj->getMesh()->ID == "TESTING_BASE_PLATE")
				obj->setIsBasePlate(true);
		obj->setIdentity(fileName);
		obj->setPosition(TF.getPosition());
		obj->setRotation(TF.getRotationAngle());
		obj->getOrientation()->setScale(TF.getScale());
		obj->updateTransforms(DT);
		//std::cout << obj->getOrientation()->getPosition().x << ", " << obj->getOrientation()->getPosition().y  << ", " << obj->getOrientation()->getPosition().z << std::endl;
		parent->AttachObject(obj);
		while (std::getline(objectFiles, dataparse))
		{
			if (dataparse == "OBJECT")
			{
				std::getline(objectFiles, dataparse);
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

				Transform TF;
				TF.setPosition(nPos);
				TF.setRotationAngle(nRot);
				TF.setScale(nScale);
				TF.update(DT);

				createChild(dataparse, obj, TF);
			}
			else if (dataparse == "LIGHT")
			{
				std::getline(objectFiles, dataparse);
				for (int i = 0; i < allLight.size(); i++)
				{
					if (dataparse == allLight[i]->getName())
					{
						obj->setLight(allLight[i]);
						//std::cout << dataparse << std::endl;
					}
				}
			}
			else if (dataparse == "PHYS")
			{
				std::getline(objectFiles, dataparse);
				for (int i = 0; i < allHitboxes.size(); i++)
				{
					if (dataparse == allHitboxes[i]->name)
					{
						PhysicsBody PB(allHitboxes[i]);
						obj->setPhysicsBody(PB);
					}
				}
				std::getline(objectFiles, dataparse);
				if (dataparse == "true")
					obj->getPhysicsBody()->collider->dynamic = true;
				else
					obj->getPhysicsBody()->collider->dynamic = false;
			}
			else if (dataparse == "TARGET")
			{
				std::getline(objectFiles, dataparse);
				for (int i = 0; i < meshList.size(); i++)
				{
					if (dataparse == meshList[i]->ID)
					{
						obj->addTarget(meshList[i]);
					}
				}
			}
			else if (dataparse == "TEXTURE")
			{
				std::getline(objectFiles, dataparse);
				for (int i = 0; i < textures.size(); i++)
				{
					if (dataparse == textures[i]->getTextureName())
					{
						obj->addTexture(textures[i]);
					}
				}
			}
		}
	}
	objectFiles.close();
}

void Game::resetMap()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			unsigned short sub = theMap->getSection(i, j)->getNumObjOnFace();
			for (int k = 0; k < sub; k++)
			{
				theMap->getSection(i, j)->getObjectOnFace(k)->setPosition(theMap->getSection(i, j)->getObjectOnFace(k)->getBasePosition());
				theMap->getSection(i, j)->getObjectOnFace(k)->setRotation(theMap->getSection(i, j)->getObjectOnFace(k)->getBaseRotation());
				theMap->getSection(i, j)->getObjectOnFace(k)->getPhysicsBody()->resetForMap();
			}
		}
	}
}

//void Game::resizeCameras(float ASPect)
//{
//	aspectRatio = ASPect;
//	for (int i = 0; i < allCams.size(); i++)
//	{
//		allCams[i]->ASPECT = ASPect;
//	}
//}

//void Game::print(int x, int y, int z, char *string)
//{
//	//set the position of the text in the window using the x and y coordinates
//	glRasterPos2f(x, y);
//	//get the length of the string to display
//	int len = (int)strlen(string);
//
//	//loop to display character by character
//	for (int i = 0; i < len; i++)
//	{
//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
//	}
//};
