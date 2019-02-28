#include "Game.h"
#include "ResourceManager.h"
#include "TextureCube.h"
#include "UI.h"

#include <vector>
#include <string>
#include <fstream>
#include <random>

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

//int activeToonRamp = 0;
//bool toonActive = false;

constexpr int frameTimeNumSamples = 600;
int frameTimeCurrSample = 0;
float frameTimeSamples[frameTimeNumSamples];

//UniformBuffer* testBuffer;

void Game::initializeGame()
{
	aspect = WINDOW_WIDTH / WINDOW_HEIGHT;

	for (int i = 0; i < frameTimeNumSamples; ++i)
		frameTimeSamples[i] = 0.016f;
	if (guiEnabled && !UI::isInit)
		UI::InitImGUI();

	Camera::init();

	Light::init();
	Light::_UBO.bind(3);

	ShaderProgram::initDefault();
	Framebuffer::initFrameBuffers();

	std::string masterFile = "_MESH_MASTER_LIST";
	loadAllMeshes(masterFile);

	masterFile = "_TEXTURE_MASTER_LIST";
	loadAllTextures(masterFile);

	masterFile = "_LIGHTS_MASTER_LIST";
	loadAllLights(masterFile);

	masterFile = "_HITBOX_MASTER_LIST";
	loadAllHitboxes(masterFile);

	masterFile = "_FRAMEBUFFER_MASTER_LIST";
	loadAllFramebuffers(masterFile);

	masterFile = "_SHADER_MASTER_LIST";
	loadAllShaders(masterFile);

	masterFile = "_CAMERA_MASTER_LIST";
	loadAllCameras(masterFile);

	masterFile = "_OBJECT_MASTER_LIST";
	loadAllObjects(masterFile);

	allSetup();

	PlayerCam->perspective(60.f, aspect, 1, 1000);
	PlayerCam->update(0);

	//masterFile = "_MESH_MASTER_LIST";
	//loadAllMeshes(masterFile);
	//
	//masterFile = "_MESH_MASTER_LIST";
	//loadAllMeshes(masterFile);


	//gbuffer.init(windowWidth, windowHeight);
	//framebuffer.addDepthTarget();
	//framebuffer.addColorTarget(GL_RGB8);
	//framebuffer.init(windowWidth, windowHeight);
	//framebufferTV.addDepthTarget();
	//framebufferTV.addColorTarget(GL_RGB8);
	//framebufferTV.init(128, 128);
	//
	//meshStan.LoadFromObj("stan_big.obj");
	//meshSphere.initMeshSphere(32U, 32U);
	//meshSkybox.initMeshSphere(32U, 32U, true);
	//meshLight.initMeshSphere(6U, 6U);
	//meshPlane.initMeshPlane(32U, 32U);
	//
	//shaderBasic.load("shader.vert", "shader.frag");
	////shaderTexture.load("shader.vert", "shaderTexture.frag");
	//shaderTextureJupiter.load("shader.vert", "shaderTextureJupiter.frag");
	//shaderTextureAlphaDiscard.load("shader.vert", "shaderTextureAlphaDiscard.frag");
	//shaderSky.load("shaderSky.vert", "shaderSky.frag");
	//shaderPassthrough.load("PassThrough.vert", "PassThrough.frag");
	//shaderGrayscale.load("PassThrough.vert", "Post/GreyscalePost.frag");
	//shaderGbuffer.load("shader.vert", "gBuffer.frag");
	//shaderPointLight.load("PassThrough.vert", "dPointLight.frag");
	//
	//ResourceManager::Shaders.push_back(&shaderBasic);
	////ResourceManager::Shaders.push_back(&shaderTexture);
	//ResourceManager::Shaders.push_back(&shaderTextureJupiter);
	//ResourceManager::Shaders.push_back(&shaderTextureAlphaDiscard);
	//ResourceManager::Shaders.push_back(&shaderSky);
	//ResourceManager::Shaders.push_back(&shaderPassthrough);
	//ResourceManager::Shaders.push_back(&shaderGrayscale);
	//ResourceManager::Shaders.push_back(&shaderGbuffer);
	//ResourceManager::Shaders.push_back(&shaderPointLight);
	//
	//uniformBufferTime.allocateMemory(sizeof(float));
	//uniformBufferTime.bind(1);
	//uniformBufferLightScene.allocateMemory(sizeof(vec4));
	//uniformBufferLightScene.bind(2);
	//
	//light.init();
	//light._UBO.bind(3);
	//
	//uniformBufferToon.allocateMemory(sizeof(int) * 4);
	//uniformBufferToon.bind(5);
	//uniformBufferToon.sendBool(false, 0);
	//
	//uniformBufferLightScene.sendVector(vec3(0.05f), 0);
	////uniformBufferLight.sendVector(vec3(1.0f), sizeof(vec4)); // Set color of light to white
	//
	//Texture* texBlack = new Texture("black.png");
	//Texture* texWhite = new Texture("white.png");
	//Texture* texYellow = new Texture("yellow.png");
	////Texture* texGray = new Texture("gray.png");
	//Texture* texEarthAlbedo = new Texture("earth.jpg");
	//Texture* texEarthEmissive = new Texture("earthEmissive.png");
	//Texture* texEarthSpecular = new Texture("earthSpec.png");
	//Texture* texRings = new Texture("saturnRings.png");
	//Texture* texMoonAlbedo = new Texture("8k_moon.jpg");
	//Texture* texJupiterAlbedo = new Texture("jupiter.png");
	//Texture* texSaturnAlbedo = new Texture("8k_saturn.jpg");
	//Texture* texCheckerboard = new Texture("checkboard.png");
	//Texture* texStanAlbedo = new Texture("stan_tex.png");
	//Texture* texStanEmissive = new Texture("stan_emit.png");
	//
	//textureToonRamp.push_back(new Texture("tinyramp.png", false));
	//textureToonRamp[0]->setWrapParameters(GL_CLAMP_TO_EDGE);
	//textureToonRamp[0]->sendTexParameters();
	//
	//std::vector<Texture*> texEarth = { texEarthAlbedo, texEarthEmissive, texEarthSpecular };
	//std::vector<Texture*> texCheckboards { texCheckerboard, texBlack, texWhite };
	//std::vector<Texture*> texSun = { texBlack, texYellow, texBlack };
	//std::vector<Texture*> texMoon = { texMoonAlbedo, texBlack, texBlack };
	//std::vector<Texture*> texJupiter = { texJupiterAlbedo, texBlack, texBlack };
	//std::vector<Texture*> texPlanet = { texWhite, texBlack, texBlack };
	//std::vector<Texture*> texSaturn = { texSaturnAlbedo, texBlack, texBlack };
	//std::vector<Texture*> texSaturnRings = { texRings, texBlack, texBlack };
	//std::vector<Texture*> texStan = { texStanAlbedo, texStanEmissive, texBlack };
	//std::vector<Texture*> texTV = { texBlack, &framebufferTV._Color._Tex[0] , texBlack };
	//
	//goStan = GameObject(&meshStan, texStan);
	//goSun = GameObject(&meshSphere, texSun);
	//goEarth = GameObject(&meshSphere, texEarth);
	//goEarthPlane = GameObject(&meshPlane, texCheckboards);
	//goMoon = GameObject(&meshSphere, texMoon);
	//goJupiter = GameObject(&meshSphere, texJupiter);
	//goJupiterMoon[0] = GameObject(&meshSphere, texMoon);
	//goJupiterMoon[1] = GameObject(&meshSphere, texMoon);
	//goSaturn = GameObject(&meshSphere, texSaturn);
	//goSaturnRings = GameObject(&meshPlane, texSaturnRings);
	//goTV = GameObject(&meshPlane, texTV);
	//
	//std::vector<std::string> skyboxTex;
	//skyboxTex.push_back("sky2/sky_c00.bmp");
	//skyboxTex.push_back("sky2/sky_c01.bmp");
	//skyboxTex.push_back("sky2/sky_c02.bmp");
	//skyboxTex.push_back("sky2/sky_c03.bmp");
	//skyboxTex.push_back("sky2/sky_c04.bmp");
	//skyboxTex.push_back("sky2/sky_c05.bmp");
	//goSkybox = GameObject(&meshSkybox, new TextureCube(skyboxTex));
	//goSkybox.setShaderProgram(&shaderSky);
	//camera2.m_pSkybox = camera.m_pSkybox = &goSkybox;
	//
	//ResourceManager::addEntity(&goStan);
	//ResourceManager::addEntity(&goSun);
	//ResourceManager::addEntity(&goEarth);
	//ResourceManager::addEntity(&goEarthPlane);
	//ResourceManager::addEntity(&goMoon);
	//ResourceManager::addEntity(&goJupiter);
	//ResourceManager::addEntity(&goJupiterMoon[0]);
	//ResourceManager::addEntity(&goJupiterMoon[1]);
	//ResourceManager::addEntity(&goSaturn);
	//ResourceManager::addEntity(&goSaturnRings);
	//ResourceManager::addEntity(&camera);
	//ResourceManager::addEntity(&camera2);
	//ResourceManager::addEntity(&goTV);	
	//
	////goLight.setShaderProgram(&shaderPointLight);
	////goLight.setMesh(&meshSphere);
	////goLight.setTextures(gbuffer.textures);
	//
	//goStan.setLocalPos(vec3(-4.0f, 10.0f, 0.0f));
	//goSun.setLocalPos(vec3(4, 5, 0));
	//goEarth.setLocalPos(vec3(-2, 0, 0));
	//goEarthPlane.setLocalPos(vec3(0, -5.0f, -50));
	//goMoon.setLocalPos(vec3(-1, 0, -1));
	//goJupiter.setLocalPos(vec3(-3, 0, 4));
	//goJupiterMoon[0].setLocalPos(vec3(-4, 0, 5));
	//goJupiterMoon[1].setLocalPos(vec3(-2, 0, 3));
	//goSaturn.setLocalPos(vec3(-2, 0, -3));
	//goSaturnRings.setLocalPos(vec3(-2, 0, -3));
	//
	//std::uniform_real_distribution<float> randomPositionX(-100.0f, 100.0f);
	//std::uniform_real_distribution<float> randomPositionY(-100.0f, 100.0f);
	//std::uniform_real_distribution<float> randomPositionZ(-100.0f, -10.0f);
	//std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);
	//std::uniform_real_distribution<float> randomScale(0.5f, 4.0f);
	//std::default_random_engine generator(std::_Random_device());
	//
	//for (int i = 0; i < 500; i++)
	//{
	//	GameObject *object = new GameObject(&meshSphere, texMoon);
	//	object->setLocalPos(vec3(randomPositionX(generator), randomPositionY(generator), randomPositionZ(generator)));
	//	object->setScale(vec3(randomScale(generator)));
	//	object->setLocalRot(vec3(randomRotation(generator), randomRotation(generator), randomRotation(generator)));
	//	object->setShaderProgram(&shaderGbuffer);
	//	ResourceManager::addEntity(object);
	//	goPlanets.push_back(object);
	//}
	//
	//goStan.setScale(2.0f);
	//goSun.setScale(1.50f);
	//goEarth.setScale(0.50f);
	//goEarthPlane.setScale(100.50f);
	//goMoon.setScale(0.25f);
	//goJupiter.setScale(1.00f);
	//goJupiterMoon[0].setScale(0.25f);
	//goJupiterMoon[1].setScale(0.20f);
	//goSaturn.setScale(1.0f);
	//goSaturnRings.setScale(2.0f);
	//goSaturnRings.setLocalRotZ(-20.0f);
	//
	//goTV			.setShaderProgram(&shaderGbuffer);
	//goStan			.setShaderProgram(&shaderGbuffer);
	//goSun			.setShaderProgram(&shaderGbuffer);
	//goEarth			.setShaderProgram(&shaderGbuffer);
	//goEarthPlane	.setShaderProgram(&shaderGbuffer);
	//goMoon			.setShaderProgram(&shaderGbuffer);
	//goJupiter		.setShaderProgram(&shaderGbuffer);
	//goJupiterMoon[0].setShaderProgram(&shaderGbuffer);
	//goJupiterMoon[1].setShaderProgram(&shaderGbuffer);
	//goSaturn		.setShaderProgram(&shaderGbuffer);
	//goSaturnRings	.setShaderProgram(&shaderGbuffer);
	   	 
	// These Render flags can be set once at the start (No reason to waste time calling these functions every frame).
	// Tells OpenGL to respect the depth of the scene. Fragments will not render when they are behind other geometry.
	glEnable(GL_DEPTH_TEST); 
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	   
	// Setup our main scene objects...
	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	//camera.perspective(90.0f, aspect, 0.05f, 1000.0f);
	//camera.setLocalPos(vec3(0.0f, 4.0f, 4.0f));
	//camera.setLocalRotX(-15.0f);
	//camera.attachFrameBuffer(&gbuffer);
	//camera.m_pClearFlag = ClearFlag::SolidColor;
	//camera.setRenderList(ResourceManager::Transforms);
	//
	//camera2.perspective(90.0f, aspect, 0.05f, 1000.0f);
	////camera2.setLocalPos();
	//camera2.setLocalRotX(-15.0f);
	//camera2.setLocalRotY(180.0f);
	//camera2.attachFrameBuffer(&framebufferTV);
	//camera2.m_pClearFlag = ClearFlag::Skybox;
	//camera2.setRenderList(ResourceManager::Transforms);
	//
	//testBuffer = new UniformBuffer(32);
	//testBuffer->bind(6);
	//testBuffer->sendVector(vec4(0.0f, 0.25f, 0.5f, 0.75f), 1);
}

void Game::update()
{
	renderShip.clear();
	updateShip.clear();
	dynamicCollisionShip.clear();
	staticCollisionShip.clear();
	lightShip.clear();
	dynamicBatchShip.clear();

	//lightShip.push_back(SUN);
	protectedLightShip(SUN);
	//lightShip.push_back(getLight("HEADLIGHT"));

	//aspect = aspect;
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	frameTimeSamples[frameTimeCurrSample] = min(deltaTime, 0.1f);
	frameTimeCurrSample = (frameTimeCurrSample + 1) % frameTimeNumSamples;
	frameTimeSamples[frameTimeCurrSample] = 1.0f;

	uniformBufferTime.sendFloat(TotalGameTime, 0);
	//Light* lit = dynamic_cast<Light*>(players[0]->getChildren().at(0)->getChildren().at(0));
	//std::cout << lit->position << std::endl;

	for (Player* P : players)
	{
		bool pUP = (keysDown['w'] || keysDown['W']);
		bool pLEFT = (keysDown['a'] || keysDown['A']);
		bool pDOWN = (keysDown['s'] || keysDown['S']);
		bool pRIGHT = (keysDown['d'] || keysDown['D']);
		bool pATTACK = (keysDown[' ']);

		P->sendInput(pUP, Player::PLAYER_IN::UP);
		//std::cout << "--1" << std::endl;
		P->sendInput(pLEFT, Player::PLAYER_IN::LEFT);
		//std::cout << "--2" << std::endl;
		P->sendInput(pDOWN, Player::PLAYER_IN::DOWN);
		//std::cout << "--3" << std::endl;
		P->sendInput(pRIGHT, Player::PLAYER_IN::RIGHT);
		//std::cout << "--4" << std::endl;
		P->sendInput(pATTACK, Player::PLAYER_IN::ATTACK);
		//std::cout << "--5" << std::endl;

		//updateShip.push_back(P);
	}

	vec2 CPOSU = vec2(players[0]->getWorldPos().x, players[0]->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(theMap->grid[j][i]);
				int sectObj = theMap->fieldObjects[j][i].size();
				//if (sectObj > 0)
				//	updateShip.push_back(theMap->fieldObjects[j][i][0]);
				//std::cout << "MAYDE IT " << std::endl;
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(theMap->fieldObjects[j][i][k]);
					protectedUpdateShip(theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}
	
	vec2 pCur, pNew;
	int PX = 0;
	int PY = 0;
	int NX = 0;
	int NY = 0;
	for (GameObject* object : updateShip)
	{
		bool setNew = false;
		switch (object->TT)
		{
		case Transform::TransformType::TYPE_Player:
			//std::cout << "PLAYER_UPDATE!" << std::endl;
		case Transform::TransformType::TYPE_Destructable:
			pCur = object->getLocalPos().xz / tileSize + vec2(0.5);
			setNew = true;
		default:
			object->update(deltaTime);
			break;
		}

		if (setNew)
		{
			pNew = object->getLocalPos().xz / tileSize + vec2(0.5);

			PX = (int)pCur.x;
			PY = (int)pCur.y;
			NX = (int)pNew.x;
			NY = (int)pNew.y;

			//if (object->TT == Transform::TransformType::TYPE_Player)
			//	std::cout << PX << ", " << PY << std::endl;
			if (PX != NX || PY != NY)
				if (NX >= 0 && NX < 100 && NY >= 0 && NY < 100)
				{
					//std::cout << NX << ", " << NY << std::endl;
					theMap->removeObj(object->mapX, object->mapY, object);
					theMap->fieldObjects[NX][NY].push_back(object);
					object->mapX = NX;
					object->mapY = NY;
				}
		}
	}

	staticCollisionShip.push_back(LEFT_WALL);
	staticCollisionShip.push_back(RIGHT_WALL);
	staticCollisionShip.push_back(UPPER_WALL);
	staticCollisionShip.push_back(LOWER_WALL);

	for (Player* P : players)
	{
		addToCollisions(P, true);
		for (int i = -1 + P->mapX; i <= 1 + P->mapX; i++)
		{
			for (int j = -1 + P->mapY; j <= 1 + P->mapY; j++)
			{
				if (j >= 0 && j < 100 && i >= 0 && i < 100)
				{
					unsigned int amnt = theMap->fieldObjects[i][j].size();
					for (unsigned int k = 0; k < amnt; k++)
						if (theMap->fieldObjects[i][j][k]->getPhysicsBody()->getHB() && !theMap->fieldObjects[i][j][k]->getPhysicsBody()->getHB()->dynamic)
							addToCollisions(theMap->fieldObjects[i][j][k], false);
				}
			}
		}
	}
	//SAT_DEBUG_LOG("\nLOOP END\n");
	//std::cout << updateShip.size() << std::endl;

	updateExternals(deltaTime);
	staticCollisions();

	mouseHandler();
	keyHandler();
}

void Game::draw()
{
	//uniformBufferTime.sendFloat(TotalGameTime, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();

	//Transform* totBase = nullptr;
	//for (int i = 0; i < (int)renderShip.size(); i++)
	//	if (renderShip[i]->TT == Transform::TransformType::TYPE_BasePlate)
	//		totBase = renderShip[i];
	//std::cout << totBase->getWorldPos() << std::endl;

	PlayerCam->setRenderList(renderShip);
	PlayerCam->cull();
	PlayerCam->render();

	//std::cout << renderShip.size() << std::endl;

	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	COMIC_EXECUTION->bind();
	sceneCapture->bindColorAsTexture(0, 0);
	sceneCapture->bindColorAsTexture(1, 1);
	sceneCapture->bindColorAsTexture(2, 2);
	sceneCapture->bindDepthAsTexture(3);
	tRamp->bind(31);

	COMIC_EXECUTION->sendUniform("uModel", SUN->getLocalToWorld(), false);
	//COMIC_EXECUTION->sendUniform("doubelCheck", 0.f);
	defLight->renderToFSQ();
	//useFirst = !useFirst;

	for (Light* LIT : lightShip)
	{
		ShaderProgram* USING = nullptr;
		LIT->update(0);
		//std::cout << LIT->getName() << std::endl;
		//if (useFirst)
		//	defLight->bindColorAsTexture(0, 4);
		//else
		//	defLight2->bindColorAsTexture(0, 4);

		//defLight->bindColorAsTexture(0, 4);
		for (Transform* C : LIT->getChildren())
		{
			if (C->TT == Transform::TransformType::TYPE_Camera)
			{
				C->setRenderList(renderShip);
				C->render();
				C->getFrameBuffer()->bindColorAsTexture(0, 4);
			}
		}
			
		bool useMesh = false;
		if (LIT->LIGHT_MESH)
			useMesh = true;
		//useMesh = false;

		//useMesh = false;

		switch(LIT->type)
		{
		case Light::LightType::Directional:
			if (useMesh)
				USING = MESHLIGHT_DEFERRED_DIRECTIONAL;
			else
				USING = COMIC_DEFERRED_DIRECTIONAL;
			break;
		case Light::LightType::Point:
			if (useMesh)
				USING = MESHLIGHT_DEFERRED_POINT;
			else
				USING = COMIC_DEFERRED_POINT;
			break;
		case Light::LightType::Spotlight:
			if (useMesh)
				USING = MESHLIGHT_DEFERRED_SPOTLIGHT;
			else
				USING = COMIC_DEFERRED_SPOTLIGHT;
			break;
		}

		USING->bind();
		USING->sendUniform("uModel", LIT->getLocalToWorld(), false);
		if (useMesh)
		{
			//if (!useFirst)
			//	defLight->bind();
			//else
			//	defLight2->bind();
			defLight->bind();
			LIT->LIGHT_MESH->draw();
			//if (!useFirst)
			//	defLight->unbind();
			//else
			//	defLight2->unbind();
			defLight->unbind();
		}
		else
		{
			defLight->renderToFSQ();
			//if (useFirst)
			//	defLight->renderToFSQ();
			//else
			//	defLight2->renderToFSQ();
		}

		//if (useFirst)
		//	defLight->unbindTexture(4);
		//else
		//	defLight2->unbindTexture(4);
		//defLight->unbindTexture(4);

		useFirst = !useFirst;

		for (Transform* C : LIT->getChildren())
		{
			if (C->TT == Transform::TransformType::TYPE_Camera)
			{
				C->getFrameBuffer()->unbindTexture(4);
			}
		}
	}

	ShaderProgram::unbind();
	sceneCapture->unbindTexture(3);
	sceneCapture->unbindTexture(2);
	sceneCapture->unbindTexture(1);
	sceneCapture->unbindTexture(0);
	tRamp->unbind(31);

	//if (useFirst)
	//	defLight->bindColorAsTexture(0, 0);
	//else
	//	defLight2->bindColorAsTexture(0, 0);

	defLight->bindColorAsTexture(0, 0);

	OUTPUT->bind();
	defLight->drawFSQ();
	OUTPUT->unbind();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	//if (useFirst)
	//	defLight->unbindTexture(0);
	//else
	//	defLight2->unbindTexture(0);
	defLight->unbindTexture(0);
	//textureToonRamp[0]->bind(31);


	// Render the scene from both camera's perspective
	// this will render all objects that are in the camera list
	//light.position = camera2.getView() * vec4(goSun.getWorldPos(), 1.0f);
	//light.update(0.0f);
	//camera2.render();
	//light.position = camera.getView() * vec4(goSun.getWorldPos(), 1.0f);
	//light.update(0.0f);
	//camera.render();
	//
	////shaderGrayscale.bind();
	//shaderPointLight.bind();
	//gbuffer.bindDepthAsTexture(0);
	//gbuffer.bindColorAsTexture(0, 1);
	//gbuffer.bindColorAsTexture(1, 2);
	//gbuffer.bindColorAsTexture(2, 3);
	//glViewport(0, 0, windowWidth, windowHeight);
	//gbuffer.drawFSQ();
	//ShaderProgram::unbind();
	//gbuffer.unbindTexture(3);
	//gbuffer.unbindTexture(2);
	//gbuffer.unbindTexture(1);
	//gbuffer.unbindTexture(0);

	if(guiEnabled)
		GUI();	

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
}

void Game::GUI()
{
	UI::Start(windowWidth, windowHeight);
	// Framerate Visualizer
	ImGui::Begin("Framerate");
	{
		float averageFramerate = 0.0f;
		for (int i = 0; i < frameTimeNumSamples; ++i)
			averageFramerate += frameTimeSamples[i];
		averageFramerate = 1.0f / ((averageFramerate-1.0f) / (frameTimeNumSamples-1));
		std::string framerate = "Framerate: " + std::to_string(averageFramerate);

		ImGui::PlotHistogram("", frameTimeSamples, frameTimeNumSamples, 0, framerate.c_str(), 0.0f, 0.1f, ImVec2(frameTimeNumSamples, 60));
	}
	ImGui::End();

	static float grayscaleAmount = 1.0f;
	if (ImGui::SliderFloat("Grayscale Amount", &grayscaleAmount, 0.0f, 1.0f))
	{
	//	shaderGrayscale.bind();
	//	shaderGrayscale.sendUniform("uAmount", grayscaleAmount);
	//	shaderGrayscale.unbind();
	}
	//
	//if (ImGui::Checkbox("Toon Shading Active", &toonActive))
	//{
	//	uniformBufferToon.sendUInt(toonActive, 0);
	//}
	//if (ImGui::SliderInt("Toon Ramp Selection", &activeToonRamp, 0, (int)textureToonRamp.size() - 1))
	//{
	//	//activeToonRamp = clamp(activeToonRamp, 0, (int)textureToonRamp.size() - 1);
	//}

	//static vec3 lightPosition = goSun.getLocalPos();
	//if (ImGui::DragFloat3("Light Position", &lightPosition[0], 0.5f))
	//{
	//	goSun.setLocalPos(lightPosition);
	//}
	//
	//if (ImGui::SliderFloat3("Attenuation", &light.constantAtten, 0.0f, 1.0f, "%.8f", 6.0f))
	//{
	//	
	//}

	//ImGui::Text("Radius: %f", light.radius);

	UI::End();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	setKeysDown(true, key);
	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[(int)key] = true;
		io.AddInputCharacter((int)key); // this is what makes keyboard input work in imgui
		// This is what makes the backspace button work
		int keyModifier = glutGetModifiers();
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}

	//switch(key)
	//{
	//case 27: // the escape key
	//	break;
	//case 'w':
	//case 'W':
	//case 'w' - 96:
	//	input.moveForward = true;
	//	break;
	//case 's':
	//case 'S':
	//case 's' - 96:
	//	input.moveBackward = true;
	//	break;
	//case 'd':
	//case 'D':
	//case 'd' - 96:
	//	input.moveRight = true;
	//	break;
	//case 'a':
	//case 'A':
	//case 'a' - 96:
	//	input.moveLeft = true;
	//	break;
	//case 'e':
	//case 'E':
	//case 'e' - 96:
	//	input.moveUp = true;
	//	break;
	//case 'q':
	//case 'Q':
	//case 'q' - 96:
	//	input.moveDown = true;
	//	break;
	//case 'l':
	//case 'L':
	//case 'l' - 96:
	//	input.rotateRight = true;
	//	break;
	//case 'j':
	//case 'J':
	//case 'j' - 96:
	//	input.rotateLeft = true;
	//	break;
	//case 'i':
	//case 'I':
	//case 'i' - 96:
	//	input.rotateUp = true;
	//	break;
	//case 'k':
	//case 'K':
	//case 'k' - 96:
	//	input.rotateDown = true;
	//	break;
	//}
}	

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	setKeysDown(false, key);

	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		int keyModifier = glutGetModifiers();
		io.KeyShift = false;
		io.KeyCtrl = false;
		io.KeyAlt = false;
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}

	if (key == 27)
		exit(1);
	//switch(key)
	//{
	//case 32: // the space bar
	//	camera.cullingActive = !camera.cullingActive;
	//	break;
	//case 27: // the escape key
	//	exit(1);
	//	break;
	//case 'w':
	//case 'W':
	//case 'w' - 96:
	//	input.moveForward = false;
	//	break;
	//case 's':
	//case 'S':
	//case 's' - 96:
	//	input.moveBackward = false;
	//	break;
	//case 'd':
	//case 'D':
	//case 'd' - 96:
	//	input.moveRight = false;
	//	break;
	//case 'a':
	//case 'A':
	//case 'a' - 96:
	//	input.moveLeft = false;
	//	break;
	//case 'e':
	//case 'E':
	//case 'e' - 96:
	//	input.moveUp = false;
	//	break;
	//case 'q':
	//case 'Q':
	//case 'q' - 96:
	//	input.moveDown = false;
	//	break;
	//case 'l':
	//case 'L':
	//case 'l' - 96:
	//	input.rotateRight = false;
	//	break;
	//case 'j':
	//case 'J':
	//case 'j' - 96:
	//	input.rotateLeft = false;
	//	break;
	//case 'i':
	//case 'I':
	//case 'i' - 96:
	//	input.rotateUp = false;
	//	break;
	//case 'k':
	//case 'K':
	//case 'k' - 96:
	//	input.rotateDown = false;
	//	break;
	//}
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		guiEnabled = !guiEnabled;
		//if (guiEnabled)
		//{
		//	glutWarpPointer((int)input.mousePosGUI.x, (int)input.mousePosGUI.y);
		//	glutSetCursor(GLUT_CURSOR_INHERIT);
		//}
		//else 
		//{
		//	input.f11 = true;
		//	glutWarpPointer(windowWidth / 2, windowHeight / 2);
		//	glutSetCursor(GLUT_CURSOR_NONE);
		//}
		if (!UI::isInit)
		{
			UI::InitImGUI();
		}
		break;
	case GLUT_KEY_F5:
		for (ShaderProgram* shader : ResourceManager::Shaders)
		{
			shader->reload();
		}
		break;
	case GLUT_KEY_CTRL_L:
		//input.ctrlL = true;
		break;
	case GLUT_KEY_CTRL_R:
		//input.ctrlR = true;
		break;
	case GLUT_KEY_SHIFT_L:
		//input.shiftL = true;
		break;
	case GLUT_KEY_SHIFT_R:
		//input.shiftR = true;
		break;
	case GLUT_KEY_ALT_L:
		//input.altL = true;
		break;
	case GLUT_KEY_ALT_R:
		//input.altR = true;
		break;
	case GLUT_KEY_UP:
		//input.moveForward = true;
		break;
	case GLUT_KEY_DOWN:
		//input.moveBackward = true;
		break;
	case GLUT_KEY_RIGHT:
		//input.moveRight = true;
		break;
	case GLUT_KEY_LEFT:
		//input.moveLeft = true;
		break;
	case GLUT_KEY_PAGE_UP:
		//input.moveUp = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		//input.moveDown = true;
		break;
	case GLUT_KEY_END:
		exit(1);
		break;
	}
}

void Game::keyboardSpecialUp(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_CTRL_L:
		//input.ctrlL = false;
		break;
	case GLUT_KEY_CTRL_R:
		//input.ctrlR = false;
		break;
	case GLUT_KEY_SHIFT_L:
		//input.shiftL = false;
		break;
	case GLUT_KEY_SHIFT_R:
		//input.shiftR = false;
		break;
	case GLUT_KEY_ALT_L:
		//input.altL = false;
		break;
	case GLUT_KEY_ALT_R:
		//input.altR = false;
		break;
	case GLUT_KEY_UP:
		//input.moveForward = false;
		break;
	case GLUT_KEY_DOWN:
		//input.moveBackward = false;
		break;
	case GLUT_KEY_RIGHT:
		//input.moveRight = false;
		break;
	case GLUT_KEY_LEFT:
		//input.moveLeft = false;
		break;
	case GLUT_KEY_PAGE_UP:
		//input.moveUp = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		//input.moveDown = false;
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
		ImGui::GetIO().MouseDown[0] = !state;
	}
	else
	{
	}

	//if(state == GLUT_DOWN) 
	//{
	//	switch(button)
	//	{
	//	case GLUT_LEFT_BUTTON:
	//
	//		break;
	//	case GLUT_RIGHT_BUTTON:
	//	
	//		break;
	//	case GLUT_MIDDLE_BUTTON:
	//
	//		break;
	//	}
	//}
	//else
	//{
	//
	//}

	if (state == GLUT_DOWN)
		mouseDown[button] = true;
	else
		mouseDown[button] = false;
}

void Game::mouseMoved(int x, int y)
{
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	mousePosition = vec2((float)x, (float)y);
}

void Game::mousePassive(int x, int y)
{
	if (!guiEnabled)
	{
		//if (input.f11)
		//{
		//	//glutWarpPointer(windowWidth / 2, windowHeight / 2);
		//	input.f11 = false;
		//	input.mousePos = vec2((float)(windowWidth / 2), (float)(windowHeight / 2));
		//	input.mousePosOld = input.mousePos;
		//}
		//else
		//{
		//	//input.mousePosOld = vec2((float)(windowWidth / 2), (float)(windowHeight / 2));
		//	input.mousePos = vec2((float)x, (float)y);
		//	input.mouseMovement += input.mousePos - input.mousePosOld;
		//	input.mousePosOld = input.mousePos;
		//}
		//
		//if (x < 100 || y < 100 || x > windowWidth - 100 || y > windowHeight - 100)
		//{
		//	glutWarpPointer(windowWidth / 2, windowHeight / 2);
		//	input.mousePosOld = vec2((float)(windowWidth / 2), (float)(windowHeight / 2));
		//	input.f11 = true;
		//}
	}
	else
	{
		//input.mousePosGUI = vec2((float)x, (float)y);
	}
}

void Game::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	PlayerCam->perspective(60.f, aspect, 1, 1000);
	PlayerCam->update(0);
	//camera.perspective(90.0f, aspect, 0.05f, 1000.0f);
	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);
	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		FB->reshape(w, h);
	}
}

void Game::setKeysDown(bool down, unsigned char key)
{
	keysDown[key] = down;
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

void Game::drawChildren(Transform * TF)
{
	switch (TF->TT)
	{
	case Transform::TransformType::TYPE_GameObject:
	case Transform::TransformType::TYPE_BasePlate:
	case Transform::TransformType::TYPE_Destructable:
	case Transform::TransformType::TYPE_Player:
		renderShip.push_back(TF);
		break;
	case Transform::TransformType::TYPE_Light:
		Light* LIT = dynamic_cast<Light*>(TF);
		//lightShip.push_back(LIT);
		protectedLightShip(LIT);
		break;
	}

	for (Transform* TF2 : TF->getChildren())
	{
		//std::cout << TF->getChildren().size() << std::endl;
		drawChildren(TF2);
	}
}

void Game::protectedUpdateShip(GameObject * GO)
{
	//if (GO == players[0])
	//	std::cout << "PLAYER BE SHIPPED!" << std::endl;
	if (updateShip.size() > 0)
		protectedAddUpdate(GO, 0, updateShip.size() - 1);
	else
		updateShip.push_back(GO);
}

void Game::protectedExternalUpdateShip(GameObject * GO)
{
	if (GO->needsUpdate && !GO->hasBeenUpdated)
	{
		if (externalUpdateShip.size() > 0)
			protectedAddExternalUpdate(GO, 0, externalUpdateShip.size() - 1);
		else
			externalUpdateShip.push_back(GO);
	}
}

void Game::protectedAddUpdate(GameObject * _GO, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (updateShip[mid] == _GO)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
		//std::cout << "ILLEGAL UPDATE SHIP" << std::endl;
	}
	else if (back - front <= 1)
	{
		if (updateShip[front] > _GO)
			updateShip.insert(updateShip.begin() + front, _GO);
		else if (updateShip[back] > _GO)
			updateShip.insert(updateShip.begin() + back, _GO);
		else
			updateShip.insert(updateShip.begin() + back + 1, _GO);
		//if (_GO == players[0])
		//	std::cout << "PLAYER SHIP RECEIVED!" << std::endl;
	}
	else if (updateShip[mid] > _GO)
	{
		protectedAddUpdate(_GO, front, mid);
	}
	else
	{
		protectedAddUpdate(_GO, mid, back);
	}
}

void Game::protectedAddExternalUpdate(GameObject * _GO, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (externalUpdateShip[mid] == _GO)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (externalUpdateShip[front] > _GO)
			externalUpdateShip.insert(externalUpdateShip.begin() + front, _GO);
		else if (externalUpdateShip[back] > _GO)
			externalUpdateShip.insert(externalUpdateShip.begin() + back, _GO);
		else
			externalUpdateShip.insert(externalUpdateShip.begin() + back + 1, _GO);
		//if (_GO == players[0])
		//	std::cout << "PLAYER SHIP RECEIVED!" << std::endl;
		_GO->hasBeenUpdated = true;
	}
	else if (externalUpdateShip[mid] > _GO)
	{
		protectedAddExternalUpdate(_GO, front, mid);
	}
	else
	{
		protectedAddExternalUpdate(_GO, mid, back);
	}
}

void Game::protectedLightShip(Light* LIT) 
{
	if (lightShip.size() > 0)
		protectedAddLight(LIT, 0, lightShip.size() - 1);
	else
		lightShip.push_back(LIT);
}

void Game::protectedAddLight(Light* LIT, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (lightShip[mid] == LIT)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
		//std::cout << "ILLEGAL LIGHT CALL" << std::endl;
	}
	else if (back - front <= 1)
	{
		if (lightShip[front] > LIT)
			lightShip.insert(lightShip.begin() + front, LIT);
		else if (lightShip[back] > LIT)
			lightShip.insert(lightShip.begin() + back, LIT);
		else
			lightShip.insert(lightShip.begin() + back + 1, LIT);
	}
	else if (lightShip[mid] > LIT)
	{
		protectedAddLight(LIT, front, mid);
	}
	else
	{
		protectedAddLight(LIT, mid, back);
	}
}

void Game::updateExternals(float dt)
{
	//std::cout << externalUpdateShip.size() << std::endl;

	for (int i = externalUpdateShip.size() - 1; i >= 0; --i)
	{
		protectedUpdateShip(externalUpdateShip[i]);
		if (!externalUpdateShip[i]->needsUpdate)
		{
			externalUpdateShip[i]->hasBeenUpdated = false;
			externalUpdateShip.erase(externalUpdateShip.begin() + i);
		}
	}
}

void Game::addToCollisions(GameObject * GO, bool dynamic)
{
	if (dynamic)
	{
		if (dynamicCollisionShip.size() > 0)
			putInDynamic(GO, 0, dynamicCollisionShip.size() - 1);
		else
			dynamicCollisionShip.push_back(GO);
	}
	else
	{
		if (staticCollisionShip.size() > 0)
			putInStatic(GO, 0, dynamicCollisionShip.size() - 1);
		else
			staticCollisionShip.push_back(GO);
	}
}

void Game::putInStatic(GameObject * _GO, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (staticCollisionShip[mid] == _GO)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (staticCollisionShip[front] > _GO)
			staticCollisionShip.insert(staticCollisionShip.begin() + front, _GO);
		else if (staticCollisionShip[back] > _GO)
			staticCollisionShip.insert(staticCollisionShip.begin() + back, _GO);
		else
			staticCollisionShip.insert(staticCollisionShip.begin() + back + 1, _GO);
	}
	else if (staticCollisionShip[mid] > _GO)
	{
		putInStatic(_GO, front, mid);
	}
	else
	{
		putInStatic(_GO, mid, back);
	}
}

void Game::putInDynamic(GameObject * _GO, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (dynamicCollisionShip[mid] == _GO)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (dynamicCollisionShip[front] > _GO)
			dynamicCollisionShip.insert(dynamicCollisionShip.begin() + front, _GO);
		else if (dynamicCollisionShip[back] > _GO)
			dynamicCollisionShip.insert(dynamicCollisionShip.begin() + back, _GO);
		else
			dynamicCollisionShip.insert(dynamicCollisionShip.begin() + back + 1, _GO);
	}
	else if (dynamicCollisionShip[mid] > _GO)
	{
		putInDynamic(_GO, front, mid);
	}
	else
	{
		putInDynamic(_GO, mid, back);
	}
}

void Game::staticCollisions()
{
	int sA = staticCollisionShip.size();
	int dA = dynamicCollisionShip.size();

	for (int i = 0; i < dA; ++i)
	{
		for (int j = 0; j < sA; ++j)
		{
			dynamicCollisionShip[i]->doCollision(staticCollisionShip[j]);
		}
	}

	for (int i = 0; i < sA; ++i)
	{
		if (staticCollisionShip[i]->needsUpdate)
		{
			protectedExternalUpdateShip(staticCollisionShip[i]);
			//std::cout << "HAHA!" << std::endl;
		}
	}

	for (int i = 0; i < dA; ++i)
	{
		if (dynamicCollisionShip[i]->needsUpdate)
		{
			protectedExternalUpdateShip(dynamicCollisionShip[i]);
		}
	}
}

void Game::batchMesh(GameObject * _GO, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (dynamicBatchShip[mid] == _GO->getMesh())
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
		//Push this into the mesh vector
	}
	else if (back - front <= 1)
	{
		if (dynamicBatchShip[front] > _GO->getMesh())
		{
			dynamicBatchShip.insert(dynamicBatchShip.begin() + front, _GO->getMesh());
			dynamicBatchShip[front]->insertMatrixAndSway(_GO->getLocalToWorld(), vec4(_GO->swingPoint, _GO->swingTime));
		}
		else if (dynamicBatchShip[back] > _GO->getMesh())
		{
			dynamicBatchShip.insert(dynamicBatchShip.begin() + back, _GO->getMesh());
			dynamicBatchShip[back]->insertMatrixAndSway(_GO->getLocalToWorld(), vec4(_GO->swingPoint, _GO->swingTime));
		}
		else
		{
			dynamicBatchShip.insert(dynamicBatchShip.begin() + back + 1, _GO->getMesh());
			dynamicBatchShip[back + 1]->insertMatrixAndSway(_GO->getLocalToWorld(), vec4(_GO->swingPoint, _GO->swingTime));
		}
	}
	else if (dynamicBatchShip[mid] > _GO->getMesh())
	{
		batchMesh(_GO, front, mid);
	}
	else
	{
		batchMesh(_GO, mid, back);
	}
}

void Game::protectedBatch(GameObject * _GO)
{
	if (_GO)
	{
		if (dynamicBatchShip.size() > 0)
		{
			batchMesh(_GO, 0, dynamicBatchShip.size() - 1);
		}
		else
		{
			dynamicBatchShip.push_back(_GO->getMesh());
			dynamicBatchShip[0]->insertMatrixAndSway(_GO->getLocalToWorld(), vec4(_GO->swingPoint, _GO->swingTime));
		}
	}
}

void Game::loadAllTextures(std::string & fileName)
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

void Game::loadAllMeshes(std::string & fileName)
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

void Game::loadAllObjects(std::string & fileName)
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

void Game::loadAllUIElements(std::string & fileName)
{
}

void Game::loadAllLights(std::string & filename)
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

void Game::loadAllHitboxes(std::string & fileName)
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

void Game::loadAllPlayerObjects(std::string & fileName)
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

void Game::loadAllMorphTargs(std::string & fileName)
{
}

void Game::loadAllShaders(std::string & fileName)
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

void Game::loadAllFramebuffers(std::string & fileName)
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

		_FB->init(windowWidth, windowHeight);
		ResourceManager::addFramebuffer(_FB);

		fbFile.close();
	}
	masterFile.close();
}

void Game::loadAllCameras(std::string & fileName)
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
				VP.y = aspect;
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
				VO.y = VO.x / aspect;
			else
				VO.y = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VO.z = std::stof(dataparse);

			std::getline(camFile, dataparse);
			VR.x = std::stof(dataparse);

			std::getline(camFile, dataparse);
			if (dataparse == "WINDOW")
				VR.y = VR.x / aspect;
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
	}
	masterFile.close();
}

void Game::createChild(std::string & fileName, Transform * parent)
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
		else
		{
			obj = new GameObject;
		}

		switch (obj->TT)
		{
		case Transform::TransformType::TYPE_Player:
		case Transform::TransformType::TYPE_Destructable:
			obj->setShaderProgram(getShader("BOBBING_SETUP"));
			break;
		default:
			obj->setShaderProgram(getShader("COMIC_SETUP"));
			break;
		}

		//std::cout << COMIC_SETUP << "_!" << std::endl;

		if (textureName == "EMPTY")
		{
			obj->setTexture(nullptr);
			hasTex = true;
		}
		else
		{
			Texture* _TEX = getTexture(textureName);
			if (_TEX)
			{
				obj->setTexture(_TEX);
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
			Mesh* _MESH = getMesh(meshName);
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
				std::cout << "Texture not found!" << std::endl;
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

					Light* LIT = getCloneOfLight(dataparse);
					if (LIT)
					{
						obj->addChild(LIT);
					}
				}
				else if(dataparse == "CAMERA")
				{
					std::getline(objectFiles, dataparse);

					Camera* CAM = getCloneOfCamera(dataparse);

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
			}
		}
	}
	objectFiles.close();
	if (parent)
		parent->addChild(obj);
}

void Game::createUniqueChild(std::string & fileName, Transform * parent)
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
	boundRadius = std::stof(BRSTR);
	bool hasTex = false, hasMesh = false;

	GameObject* obj; //= new GameObject;

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
	else
	{
		obj = new GameObject;
	}

	switch (obj->TT)
	{
	case Transform::TransformType::TYPE_Player:
	case Transform::TransformType::TYPE_Destructable:
		obj->setShaderProgram(getShader("BOBBING_SETUP"));
		break;
	default:
		obj->setShaderProgram(getShader("COMIC_SETUP"));
		break;
	}

	if (textureName == "EMPTY")
	{
		obj->setTexture(nullptr);
		hasTex = true;
	}
	else
	{
		Texture* _TEX = getTexture(textureName);
		if (_TEX)
		{
			obj->setTexture(_TEX);
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
		Mesh* _MESH = getMesh(meshName);
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
			std::cout << "Texture not found!" << std::endl;
		if (!hasMesh)
			std::cout << "Mesh not found!" << std::endl;
		delete obj;
		obj = nullptr;
	}
	else
	{
		obj->setName(fileName);
		ResourceManager::addEntityINGAME(obj);
		while (std::getline(objectFiles, dataparse))
		{
			if (dataparse == "OBJECT")
			{
				std::getline(objectFiles, dataparse);

				createUniqueChild(dataparse, obj);
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
				
				Light* LIT = getCloneOfLight(dataparse);
				if (LIT)
				{
					obj->addChild(LIT);
				}
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
		}
	}
	objectFiles.close();
	if (parent)
		parent->addChild(obj);
}

void Game::allSetup()
{
	uniformBufferTime.allocateMemory(sizeof(float));
	uniformBufferTime.bind(1);

	uRes.allocateMemory(sizeof(vec4));
	uRes.bind(4);

	tRamp = getTexture("Game Toon Ramp");

	setBaseAndBoundaries();
	setShaders();
	setFramebuffers();
	setCamerasAndPlayers();
	generateMap();
}

void Game::setBaseAndBoundaries()
{
	UPPER_WALL = getCloneOfBoundary("TOP_AND_BOT_COLLIDER");
	LOWER_WALL = getCloneOfBoundary("TOP_AND_BOT_COLLIDER");
	LEFT_WALL = getCloneOfBoundary("LEFT_AND_RIGHT_COLLIDER");
	RIGHT_WALL = getCloneOfBoundary("LEFT_AND_RIGHT_COLLIDER");

	UPPER_WALL->setLocalPos(vec3(297.f, 0, -9.f));
	LOWER_WALL->setLocalPos(vec3(297.f, 0, 603.f));
	LEFT_WALL->setLocalPos(vec3(-9.f, 0, 297.f));
	RIGHT_WALL->setLocalPos(vec3(603.f, 0, 297.f));
	SUN = getLight("SUNLIGHT");

	overlay = getTexture("Medium shading");
	//FIELD_SAND
	//	FIELD_ROAD_T
	//	FIELD_ROAD_STRAIGHT
	//	FIELD_ROAD_CORNER
	//	FIELD_ROAD_4WAY
	//	FIELD_GRASS
	//	FIELD_DIRT
	//	FIELD_CONCRETE
	BASE_PLATE_SAND = getBasePlate("FIELD_SAND");
	BASE_PLATE_T = getBasePlate("FIELD_ROAD_T");
	BASE_PLATE_ROAD = getBasePlate("FIELD_ROAD_STRAIGHT");
	BASE_PLATE_CORNER = getBasePlate("FIELD_ROAD_CORNER");
	BASE_PLATE_4WAY = getBasePlate("FIELD_ROAD_4WAY");
	BASE_PLATE_GRASS = getBasePlate("FIELD_GRASS");
	BASE_PLATE_DIRT = getBasePlate("FIELD_DIRT");
	BASE_PLATE_CONCRETE = getBasePlate("FIELD_CONCRETE");
}

void Game::setShaders()
{
	//PassThrough = getShader("MAIN_RENDER");
	EXPLOSIONSHADER = getShader("EXPLOSION");
	MINESHADER = getShader("MINES");
	COMIC_MINE = getShader("COMIC_MINE");
	COMIC_EXPLOSION = getShader("COMIC_EXPLOSION");
	COMIC_SETUP = getShader("COMIC_SETUP");
	COMIC_EXECUTION = getShader("COMIC_EXECUTION");
	COMIC_DEFERRED_DIRECTIONAL = getShader("COMIC_DEFERRED_DIRECTIONAL");
	COMIC_DEFERRED_POINT = getShader("COMIC_DEFERRED_POINT");
	COMIC_DEFERRED_SPOTLIGHT = getShader("COMIC_DEFERRED_SPOTLIGHT");
	MESHLIGHT_DEFERRED_DIRECTIONAL = getShader("LIGHTMESH_DEFERRED_DIRECTIONAL");
	MESHLIGHT_DEFERRED_POINT = getShader("LIGHTMESH_DEFERRED_POINT");
	MESHLIGHT_DEFERRED_SPOTLIGHT = getShader("LIGHTMESH_DEFERRED_SPOTLIGHT");
	OUTPUT = getShader("JUST_OUTPUT");
	BOUNCE_SETUP = getShader("BOBBING_SETUP");
}

void Game::setFramebuffers()
{
	sceneCapture = getFramebuffer("INITIAL_SCREEN");
	defLight = getFramebuffer("DEF_LIGHT");
	defLight2 = getFramebuffer("DEF_LIGHT_2");
}

void Game::setCamerasAndPlayers()
{
	//GameObject* playerContainer = new GameObject;
	//playerContainer->addChild(getPlayer("PLAYER_TRUCK"));
	//players.push_back(playerContainer);

	PlayerCam = getCloneOfCamera("PLAYER_CAM");
	PlayerCam->setRenderList(renderShip);
	//PlayerCam->attachFrameBuffer(sceneCapture);
	//PlayerCam->perspective(60.f, aspect, 1.0f, 1000.f);
}

void Game::generateMap()
{
	std::string MAP_DIRECTORY = "TEST_MAP.txt";
	theMap = new Field(MAP_DIRECTORY);

	players = theMap->players;
	players[0]->addChild(PlayerCam);

	float camHeight = 20.f;

	PlayerCam->setLocalPos(vec3(0, camHeight, camHeight / (float)sqrt(3)));
	PlayerCam->setLocalRot(vec3(-60.f, 0, 0));

	for (Transform* object : ResourceManager::Transforms)
	{
		object->update(0);
	}

	for (Transform* object : ResourceManager::TransformsINGAME)
	{
		object->update(0);
	}

	for (Player* PL : players)
	{
		PL->playerInit(Player::PLAYER_TYPE::TRUCK);
	}
}

Boundary * Game::getBoundary(std::string _NAME)
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

Destructable * Game::getDestructable(std::string _NAME)
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

Player * Game::getPlayer(std::string _NAME)
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

BasePlate * Game::getBasePlate(std::string _NAME)
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

Light * Game::getLight(std::string _NAME)
{
	Light* SUB = ResourceManager::searchForLight(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Hitbox2D * Game::getHitbox(std::string _NAME)
{
	Hitbox2D* SUB = ResourceManager::searchForHitbox(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Mesh * Game::getMesh(std::string _NAME)
{
	Mesh* SUB = ResourceManager::searchForMesh(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Texture * Game::getTexture(std::string _NAME)
{
	Texture* SUB = ResourceManager::searchForTexture(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Camera * Game::getCamera(std::string _NAME)
{
	Camera* SUB = ResourceManager::searchForCamera(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Transform * Game::getEntity(std::string _NAME)
{
	Transform* SUB = ResourceManager::searchForTransform(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

ShaderProgram * Game::getShader(std::string _NAME)
{
	ShaderProgram* SUB = ResourceManager::searchForShader(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

GameObject * Game::getObject(std::string _NAME)
{
	GameObject* SUB = ResourceManager::searchForGameObject(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Framebuffer * Game::getFramebuffer(std::string _NAME)
{
	Framebuffer* SUB = ResourceManager::searchForFramebuffer(_NAME);
	if (!SUB)
		SAT_DEBUG_LOG("%s MISSING!", _NAME.c_str());
	return SUB;
}

Boundary * Game::getCloneOfBoundary(std::string _NAME)
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

Destructable * Game::getCloneOfDestructable(std::string _NAME)
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

Player * Game::getCloneOfPlayer(std::string _NAME)
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

BasePlate * Game::getCloneOfBasePlate(std::string _NAME)
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

Light * Game::getCloneOfLight(std::string _NAME)
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

Camera * Game::getCloneOfCamera(std::string _NAME)
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

Transform * Game::getCloneOfEntity(std::string _NAME)
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

GameObject * Game::getCloneOfObject(std::string _NAME)
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

void Game::cloneChildren(Transform * _TF)
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
