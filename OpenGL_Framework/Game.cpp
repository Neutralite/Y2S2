#include "Game.h"
#include "ResourceManager.h"
#include "TextureCube.h"
#include "UI.h"

#include <vector>
#include <string>
#include <fstream>
#include <random>

#include <cstdlib>
#include <ctime>
#include "ExternalPlayerData.h"

Game::Game()
{
	updateTimer = new Timer();
	gameCheckTimer = new Timer();
	srand((unsigned int)time(0));


	RADIAL_POST_PROC.setFormat(GL_RGB8);
	RADIAL_POST_PROC.init(windowWidth, windowHeight);
}

Game::~Game()
{
	delete updateTimer;
	delete gameCheckTimer;
}

void Game::initializeGame()
{
	//std::cout << "INITING" << std::endl;
	readyToTerminate = false;
	isInScene = false;
	TotalTime = 0.f;
	dropOffTimer = 0.f;

	EPD::bootScreens();

	minutes = 3;
	seconds = 0;

	pScore = 0;
	lerpScore = 0;

	tTime = 1.0001f;

	players.clear();
	players.resize(4);

	PlayerCams.clear();
	PlayerCams.resize(4);

	SUNS.clear();
	SUNS.resize(4);

	UIcams.clear();
	UIcams.resize(4);

	TUI.clear();
	TUI.resize(4);
	TUI2.clear();
	TUI2.resize(4);

	smartScale.clear();
	smartScale.resize(5, vec3());

	renderShips.clear();
	UITextShips.clear();
	UIRenderShips.clear();
	lightShips.clear();
	shadowShips.clear();

	std::vector<Transform*> subStep;
	std::vector<Text*> textStep;
	std::vector<Light*> lightStep;
	std::vector<GameObject*> shadowStep;

	renderShips.resize(4);
	UITextShips.resize(4);
	UIRenderShips.resize(4);
	lightShips.resize(4);
	shadowShips.resize(4);

	radialBlur.clear();
	radialBlur.resize(4, 0.f);

	allSetup();

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			//PlayerCams[i]->perspective(60.f, aspect, 1, 1000);
			PlayerCams[i]->update(0);
			UIcams[i]->update(0);
			renderShips[i] = subStep;
			UITextShips[i] = textStep;
			UIRenderShips[i] = subStep;
			lightShips[i] = lightStep;
			shadowShips[i] = shadowStep;
		}
	}

	glEnable(GL_DEPTH_TEST); 
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	updateTimer->tick();
	deltaTime = updateTimer->getElapsedTimeSeconds();


	for (unsigned int i = 0; i < 256; i++)
	{
		setKeysDown(false, (unsigned char)i);
	}

	_GS = GS_BEGINNING;
}

void Game::update()
{
	if (!isInScene)
	{
		updateTimer->tick();
		isInScene = true;
	}

	//std::cout << deltaTime << std::endl;
	//getTime = true;
	if (gameFrame > 600)
	{
		getTime = false;
	}
	
	_INPUT.update();

	for (int i = 0; i < 4; i++)
	{
		if (_INPUT.controllerConnected(i))
			controllers[i] = _INPUT.getController(i);
		else
			controllers[i] = nullptr;
	}

	gameFrame++;

	//std::cout << std::endl;
	if (getTime)
	{
		std::cout << "FRAME: " << gameFrame << std::endl;
	}

	updateShip.clear();
	dynamicCollisionShip.clear();
	staticCollisionShip.clear();
	dynamicBatchShip.clear();
	textShip.clear();

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			renderShips[i].clear();
			UITextShips[i].clear();
			UIRenderShips[i].clear();
			lightShips[i].clear();
			shadowShips[i].clear();

			PlayerCams[i]->setRenderList(renderShips[i]);
		}
	}

	updateTimer->tick();
	//gameCheckTimer->tick();
	//std::cout << ";   INITIAL: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;

	deltaTime = updateTimer->getElapsedTimeSeconds();

	frameTimeSamples[frameTimeCurrSample] = min(deltaTime, 0.1f);
	frameTimeCurrSample = (frameTimeCurrSample + 1) % frameTimeNumSamples;
	frameTimeSamples[frameTimeCurrSample] = 1.0f;

	switch (_GS)
	{
	case GS_BEGINNING:
		beginningUpdate(deltaTime);
		break;
	case GS_STARTING:
		startingUpdate(deltaTime);
		break;
	case GS_RUNNING:
		runningUpdate(deltaTime);
		break;
	case GS_PAUSED:
		pausedUpdate(deltaTime);
		break;
	case GS_ENDED:
		endedUpdate(deltaTime);
		break;
	case GS_EXITING:
		exitingUpdate(deltaTime);
		break;
	}

	TotalTime += deltaTime;

	uniqueKeyPresses();

	//mouseHandler();
	//keyHandler();

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   POST-UPDATE: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}
}

void Game::draw()
{
	glViewport(0, 0, windowWidth, windowHeight);
	//uniformBufferTime.sendFloat(TotalGameTime, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();


	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			pDraw(i);
		}
	}

	glViewport(0, 0, windowWidth, windowHeight);

	transition->bindColorAsTexture(0, 0);
	tSwitch->bind(1);
	tFade->bind(2);
	transOver->bind(3);
	TRANSITION_SHADER->bind();
	TRANSITION_SHADER->sendUniform("tTime", tTime);

	Framebuffer::drawFSQ();

	TRANSITION_SHADER->unbind();
	transOver->unbind(3);
	tFade->unbind(2);
	tSwitch->unbind(1);
	transition->unbindTexture(0);

	ShaderProgram::unbind();

	//RADIAL_POST_PROC.reshape(windowWidth, windowHeight);

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   POST-PROCESSING-DRAW: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}

	if(guiEnabled)
		GUI();	

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   GUI-DRAW: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   BUFFER-SWAP: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}
}

void Game::pDraw(int pNum)
{
	//uniformBufferTime.sendFloat(TotalGameTime, 0);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//for (Framebuffer* _FB : ResourceManager::allFramebuffers)
	//	_FB->clear();
	vec2 sSize = EPD::screenDims * vec2((float)windowWidth, (float)windowHeight);

	vec4 pS = EPD::screenRats[pNum] *
		vec4((float)windowWidth, (float)windowHeight,
		(float)windowWidth, (float)windowHeight);

	sceneCapture->clear();
	UI_SCREEN->clear();
	defLight->clear();
	RADIAL_POST_PROC.clear();


	//vec4 RES = vec4((float)windowWidth, (float)windowHeight,
	//	1.f / (float)windowWidth, 1.f / (float)windowHeight);
	vec4 RES = vec4(sSize.x, sSize.y, 1.f / sSize.x, 1.f / sSize.y);
	uRes.sendVector(RES, 0);

	//std::cout << pS << std::endl;
	//glViewport((unsigned int)pS.x, (unsigned int)pS.y,
	//	(unsigned int)(pS.z - pS.x), (unsigned int)(pS.w - pS.y));
	glViewport(0, 0, (unsigned int)sSize.x, (unsigned int)sSize.y);

	//Transform* totBase = nullptr;
	//for (int i = 0; i < (int)renderShip.size(); i++)
	//	if (renderShip[i]->TT == Transform::TransformType::TYPE_BasePlate)
	//		totBase = renderShip[i];
	//std::cout << totBase->getWorldPos() << std::endl;

	//vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	//vec2 sSize = EPD::screenDims * vec2((float)windowWidth, (float)windowHeight);

	PlayerCams[pNum]->sendUBO();
	PlayerCams[pNum]->setRenderList(renderShips[pNum]);
	UIcams[pNum]->setRenderList(UIRenderShips[pNum]);
	PlayerCams[pNum]->cull();
	UIcams[pNum]->cull();

	
	//std::cout << "-------------------------\n" << std::endl;

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   CULLING: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}

	PlayerCams[pNum]->render();

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   STATIC-DRAW: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}
	//std::cout << renderShip.size() << std::endl;

	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	COMIC_EXECUTION->bind();
	sceneCapture->bindColorAsTexture(0, 0);
	sceneCapture->bindColorAsTexture(1, 1);
	sceneCapture->bindColorAsTexture(2, 2);
	sceneCapture->bindColorAsTexture(3, 4);
	sceneCapture->bindColorAsTexture(4, 4);
	sceneCapture->bindDepthAsTexture(5);
	tRamp->bind(31);

	COMIC_EXECUTION->sendUniform("uModel", SUNS[pNum]->getLocalToWorld(), false);
	defLight->renderToFSQ();

	for (Light* LIT : lightShips[pNum])
	{
		ShaderProgram* USING = nullptr;
		LIT->update(0);

		for (Transform* C : LIT->getChildren())
		{
			if (C->TT == Transform::TransformType::TYPE_Camera)
			{
				C->setRenderList(renderShips[pNum]);
				C->render();
				C->getFrameBuffer()->bindColorAsTexture(0, 4);
			}
		}

		bool useMesh = false;
		if (LIT->LIGHT_MESH)
			useMesh = true;

		switch (LIT->type)
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
			defLight->bind();
			LIT->LIGHT_MESH->draw();
			defLight->unbind();
		}
		else
		{
			defLight->renderToFSQ();
		}

		useFirst = !useFirst;

		for (Transform* C : LIT->getChildren())
		{
			if (C->TT == Transform::TransformType::TYPE_Camera)
			{
				C->getFrameBuffer()->unbindTexture(4);
				C->getFrameBuffer()->clear();
			}
		}
	}

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   LIGHT-DRAW: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}

	ShaderProgram::unbind();
	sceneCapture->unbindTexture(5);
	sceneCapture->unbindTexture(4);
	sceneCapture->unbindTexture(3);
	sceneCapture->unbindTexture(2);
	sceneCapture->unbindTexture(1);
	sceneCapture->unbindTexture(0);
	tRamp->unbind(31);

	sceneCapture->bindColorAsTexture(0, 0);
	sceneCapture->bindColorAsTexture(2, 1);
	sceneCapture->bindColorAsTexture(3, 2);
	sceneCapture->bindColorAsTexture(4, 3);
	defLight->bindColorAsTexture(0, 4);
	defLight->bindColorAsTexture(1, 5);
	defLight->bindColorAsTexture(2, 6);
	difOver->bind(30);
	tDiffuse->bind(31);

	//RADIAL_POST_PROC.reshape((radialHeight * windowWidth) / windowHeight, radialHeight);
	float pseudoAsp = EPD::screenDims.x / EPD::screenDims.y;
	COMBINED_DRAW->bind();
	COMBINED_DRAW->sendUniform("texRot", 0.4f);
	COMBINED_DRAW->sendUniform("ASPECT", vec2(pseudoAsp * (float)windowWidth, (float)windowHeight));

	RADIAL_POST_PROC.drawToPost();
	COMBINED_DRAW->unbind();

	if (getTime)
	{
		gameCheckTimer->tick();
		std::cout << ";   DEFERRED-DRAW: " << gameCheckTimer->getElapsedTimeSeconds() << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	//if (useFirst)
	//	defLight->unbindTexture(0);
	//else
	//	defLight2->unbindTexture(0);

	tDiffuse->unbind(31);
	overlay->unbind(30);
	defLight->unbindTexture(6);
	defLight->unbindTexture(5);
	defLight->unbindTexture(4);
	sceneCapture->unbindTexture(3);
	sceneCapture->unbindTexture(2);
	sceneCapture->unbindTexture(1);
	sceneCapture->unbindTexture(0);

	RADIAL_BLUR->bind();
	RADIAL_BLUR->sendUniform("uAngle", radialBlur[pNum]);
	RADIAL_BLUR->sendUniform("uCenter", vec2(0.5f, 0.5f));

	for (int i = 0; i < radialLoops; i++)
	{
		RADIAL_POST_PROC.draw();
	}

	OUTPUT->bind();

	//RES = vec4(sSize.x, sSize.y, 1.f / sSize.x, 1.f / sSize.y);
	//uRes.sendVector(RES, 0);
	//glViewport(0, 0, windowWidth, windowHeight);


	RES = vec4((float)windowWidth, (float)windowHeight,
		1.f / (float)windowWidth, 1.f / (float)windowHeight);
	//vec4 RES = vec4(sSize.x, sSize.y, 1.f / sSize.x, 1.f / sSize.y);
	uRes.sendVector(RES, 0);

	//std::cout << pS << std::endl;
	glViewport((unsigned int)pS.x, (unsigned int)pS.y,
		(unsigned int)(pS.z - pS.x), (unsigned int)(pS.w - pS.y));

	RADIAL_POST_PROC.drawToFB(transition);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	UIcams[pNum]->sendUBO();


	UIcams[pNum]->renderToFB(transition, false);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);

	//std::cout << PlayerCams[pNum]->getProjection() << std::endl;
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

	UI::End();
}

void Game::releaseScene()
{
	delete theMap;
	rm::order66();

	players.clear();
	renderShips.clear();
	UIRenderShips.clear();
	updateShip.clear();
	externalUpdateShip.clear();
	dynamicCollisionShip.clear();
	staticCollisionShip.clear();
	RE_SPAWN.clear();
	lightShips.clear();
	shadowShips.clear();
	dynamicBatchShip.clear();
	weaponShip.clear();
	textShip.clear();
	UITextShips.clear();
	pTotals.clear();
	pFloats.clear();
}

void Game::beginningUpdate(float dt)
{
	tTime -= deltaTime * 1.f;
	//std::cout << tTime << std::endl;
	if (tTime < 0.f)
	{
		_GS = GS_STARTING;
		tTime = 0.f;
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	timeString = std::to_string((int)seconds);
	for (unsigned int i = 0; i < TIMER->messageSize(); i++)
	{
		TIMER->posOffset[i] = vec3(0.f);
		TIMER2->posOffset[i] = vec3(0.f);
	}
	if (timeString.size() < 2)
	{
		timeString = std::to_string(minutes) + ":0" + std::to_string((int)seconds);
	}
	else
	{
		timeString = std::to_string(minutes) + ":" + std::to_string((int)seconds);
	}

	if (minutes < 0 || minutes == 0 && seconds == 0)
	{
		timeString = "TIME";
	}

	TIMER2->baseColor = vec3(180.f, 60.f * minutes + seconds, 60.f * minutes + seconds) / 180.f;
	TIMER->setMessage(timeString);
	TIMER2->setMessage(timeString);

	if (minutes == 0 && seconds > 0)
	{
		for (unsigned int i = 0; i < TIMER->messageSize(); i++)
		{
			TIMER->posOffset[i] = vec3(sin(TotalGameTime * 34.5f + i), cos(TotalGameTime * 38.3f + i), 0.f) * 0.07f;
			TIMER2->posOffset[i] = vec3(sin(TotalGameTime * 34.5f + i), cos(TotalGameTime * 38.3f + i), 0.f) * 0.07f;
		}
	}

	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		UITextShips[i].push_back(TIMER);
		UITextShips[i].push_back(TIMER2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			beginningUpdatePlayer(dt, i);
		}
	}

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		textShip[i]->update(deltaTime);
	}
}

void Game::startingUpdate(float dt)
{
	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	_GS = GS_RUNNING;


	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		UITextShips[i].push_back(TIMER);
		UITextShips[i].push_back(TIMER2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			startingUpdatePlayer(dt, i);
		}
	}

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		textShip[i]->update(deltaTime);
	}
}

void Game::runningUpdate(float dt)
{
	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		pFloats[i] -= deltaTime;
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);

			pTotals[i]->setLocalPos(pTotals[i]->getLocalPos() + vec3(0, deltaTime * 2.f, 0));
			pTotals[i]->aS = vec3(pow(pFloats[i] * (pMax - pFloats[i]) / pMax / pMax, 0.1f)) * 1.f;
			for (unsigned int j = 0; j < pTotals[i]->messageSize(); j++)
			{
				pTotals[i]->colorShift[j] = pow(vec3(cos(pFloats[i] * 6.f), cos(pFloats[i] * 6.f), 1), 2);
			}
		}
	}

	seconds -= deltaTime;

	while (seconds < 0.f)
	{
		minutes--;
		seconds += 60.f;
	}

	timeString = std::to_string((int)seconds);
	for (unsigned int i = 0; i < TIMER->messageSize(); i++)
	{
		TIMER->posOffset[i] = vec3(0.f);
		TIMER2->posOffset[i] = vec3(0.f);
	}
	if (timeString.size() < 2)
	{
		timeString = std::to_string(minutes) + ":0" + std::to_string((int)seconds);
	}
	else
	{
		timeString = std::to_string(minutes) + ":" + std::to_string((int)seconds);
	}

	if (minutes < 0 || minutes == 0 && seconds == 0)
	{
		timeString = "TIME";
		_GS = GS_ENDED;
	}

	TIMER2->baseColor = vec3(180.f, 60.f * minutes + seconds, 60.f * minutes + seconds) / 180.f;
	TIMER->setMessage(timeString);
	TIMER2->setMessage(timeString);

	if (minutes == 0 && seconds > 0)
	{
		for (unsigned int i = 0; i < TIMER->messageSize(); i++)
		{
			TIMER->posOffset[i] = vec3(sin(TotalGameTime * 34.5f + i), cos(TotalGameTime * 38.3f + i), 0.f) * 0.07f;
			TIMER2->posOffset[i] = vec3(sin(TotalGameTime * 34.5f + i), cos(TotalGameTime * 38.3f + i), 0.f) * 0.07f;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			UITextShips[i].push_back(TIMER);
			UITextShips[i].push_back(TIMER2);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			runningUpdatePlayer(dt, i);
		}
	}
	performUpdates(deltaTime);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		textShip[i]->update(deltaTime);
	}

	updateExternals(deltaTime);
	updateAttacks(deltaTime);
	staticCollisions();
}

void Game::pausedUpdate(float dt)
{
	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		UITextShips[i].push_back(TIMER);
		UITextShips[i].push_back(TIMER2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			pausedUpdatePlayer(dt, i);
		}
	}

	updateExternals(0);
	updateAttacks(0);
}

void Game::endedUpdate(float dt)
{
	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		UITextShips[i].push_back(TIMER);
		UITextShips[i].push_back(TIMER2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			endedUpdatePlayer(dt, i);
		}
	}

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		textShip[i]->update(deltaTime);
	}

	updateExternals(0);
	updateAttacks(0);

	dropOffTimer += deltaTime;
	if (dropOffTimer > 1.f)
	{
		dropOffTimer = 1.f;
		_GS = GS_EXITING;
		tTime = 0.f;

		tSwitch = rm::getTexture("SCENE_GAME_END");
		tFade = rm::getTexture("defaultWhite");
	}
}

void Game::exitingUpdate(float dt)
{
	for (int i = pTotals.size() - 1; i >= 0; --i)
	{
		if (pFloats[i] <= 0.f)
		{
			pFloats.erase(pFloats.begin() + i);
			rm::destroyObjectINGAME(pTotals[i]);
			pTotals.erase(pTotals.begin() + i);
		}
		else
		{
			textShip.push_back(pTotals[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		UITextShips[i].push_back(TIMER);
		UITextShips[i].push_back(TIMER2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			exitingUpdatePlayer(dt, i);
		}
	}

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		textShip[i]->update(deltaTime);
	}

	if (tTime < 1.0001f)
	{
		tTime += deltaTime * 0.7f;
	}
	else
	{
		tTime = 1.f;
		for (int i = 0; i < 4; i++)
		{
			if (EPD::playerActive[i])
			{
				EPD::playerScores[i] = (float)players[i]->POINT_TOTAL;
				//EPD::
			}
		}
		readyToTerminate = true;
		nextScene = "VICTORY";
	}
}

void Game::beginningUpdatePlayer(float dt, int pNum)
{
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);

	vec2 CPOSU = vec2(players[pNum]->getWorldPos().x, players[pNum]->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	scoreLerp(deltaTime, pNum);


	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		UITextShips[pNum][i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}
}

void Game::startingUpdatePlayer(float dt, int pNum)
{
	_GS = GS_RUNNING;
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);

	vec2 CPOSU = vec2(players[pNum]->getWorldPos().x, players[pNum]->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	scoreLerp(deltaTime, pNum);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		UITextShips[pNum][i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}

}

void Game::runningUpdatePlayer(float dt, int pNum)
{
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);
	//SUN->rotateBy(deltaTime * 9.f, normalize(vec3(1, 0, 1)));
	//SUN->intensity = pow(max(dot(normalize(mat3(SUN->getLocalToWorld()) * vec3(SUN->direction)), vec3(0, -1.f, 0)), 0.f), 0.6f);
	//SUN->update(deltaTime);

	Player* P = players[pNum];


	//std::cout << P->getWeapon() << std::endl;

	bool pUP;
	bool pLEFT;
	bool pDOWN;
	bool pRIGHT;
	bool pATTACK;

	if (controllers[pNum])
	{
		controllers[pNum]->getSticks(&playerInput[2 * pNum], &playerInput[2 * pNum + 1]);
		controllers[pNum]->getTriggers(&playerTriggers[pNum]);

		float cDist = sqrt(pow(playerInput[2 * pNum].y, 2) + pow(playerInput[2 * pNum].x, 2));

		pUP = (playerTriggers[pNum].RT > 0.5f || (playerInput[2 * pNum].y > 0.f && cDist > 0.2f
			&& !(playerTriggers[pNum].RT > 0.5f) && !(playerTriggers[pNum].LT > 0.5f)));
		pLEFT = (playerInput[2 * pNum].x < -0.2f);
		pDOWN = (playerTriggers[pNum].LT > 0.5f || (playerInput[2 * pNum].y < -0.1f && cDist > 0.2f
			&& !(playerTriggers[pNum].RT > 0.5f) && !(playerTriggers[pNum].LT > 0.5f)));
		pRIGHT = (playerInput[2 * pNum].x > 0.2f);
		pATTACK = (controllers[pNum]->isButtonPressed(A));

		P->steeringMultiplier = abs(playerInput[2 * pNum].x);
	}
	else
	{
		pUP = (keysDown['w'] || keysDown['W']);
		pLEFT = (keysDown['a'] || keysDown['A']);
		pDOWN = (keysDown['s'] || keysDown['S']);
		pRIGHT = (keysDown['d'] || keysDown['D']);
		pATTACK = (keysDown[' ']);
	}
	//bool pATTACK = (keysDown[' ']);

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

	if (P->sendATTACK)
	{
		generateATTACK(P);
	}

	//std::cout << players[i]->getLocalPos() << std::endl;
	//std::cout << players[i]->getPhysicsBody()->getVelocity() << std::endl;
	//std::cout << players[i]->getPhysicsBody()->getAcceleration() << std::endl;
	//
	//std::cout << PlayerCam->getLocalPos() << std::endl;

	vec2 CPOSU = vec2(players[pNum]->getWorldPos().x, players[pNum]->getWorldPos().z) / tileSize;

	//std::cout << CPOSU << std::endl;
	//std::cout << players[pNum]->mapX << ", " << players[pNum]->mapY << std::endl;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	//std::cout << updateShip.size() << std::endl;
	//std::cout << "TEST1" << std::endl;
	//performUpdates(deltaTime);
	//std::cout << "____PASS!" << std::endl;

	staticCollisionShip.push_back(LEFT_WALL);
	staticCollisionShip.push_back(RIGHT_WALL);
	staticCollisionShip.push_back(UPPER_WALL);
	staticCollisionShip.push_back(LOWER_WALL);

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

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	scoreLerp(deltaTime, pNum);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		//if (i == textShip.size() - 1)
		//	std::cout << textShip[i]->getLocalPos() << std::endl;
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		UITextShips[pNum][i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}

	radialBlur[pNum] += abs(P->getAngularVelocity().y * 0.001f) * deltaTime;
	radialBlur[pNum] = lerp(0.f, radialBlur[pNum], pow(0.90f, 60.f * deltaTime));
}

void Game::pausedUpdatePlayer(float dt, int pNum)
{
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);

	Player* P = players[pNum];

	vec2 CPOSU = vec2(P->getWorldPos().x, P->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					//protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		//textShip[i]->update(deltaTime);
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		//UITextShip[i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}
}

void Game::endedUpdatePlayer(float dt, int pNum)
{
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);

	Player* P = players[pNum];

	vec2 CPOSU = vec2(P->getWorldPos().x, P->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	scoreLerp(deltaTime, pNum);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		//textShip[i]->update(deltaTime);
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		//UITextShip[i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}
}

void Game::exitingUpdatePlayer(float dt, int pNum)
{
	protectedEntityShip(&lightShips[pNum], SUNS[pNum]);

	vec2 CPOSU = vec2(players[pNum]->getWorldPos().x, players[pNum]->getWorldPos().z) / tileSize;

	for (int i = (int)(-7 + CPOSU.y); i < 8 + CPOSU.y; i++ /*int i = -3 + camPosUnit.y; i < 1 + camPosUnit.y; i++*/)
	{
		for (int j = (int)(-7 + CPOSU.x); j < 8 + CPOSU.x; j++)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->grid[j][i], true);
				int sectObj = theMap->fieldObjects[j][i].size();
				for (int k = 0; k < sectObj; k++)
				{
					drawChildren(&renderShips[pNum], &lightShips[pNum], theMap->fieldObjects[j][i][k], true);
					protectedEntityShip(&updateShip, theMap->fieldObjects[j][i][k]);
				}
			}
		}
	}

	UITextShips[pNum].push_back(TUI[pNum]);
	UITextShips[pNum].push_back(TUI2[pNum]);

	scoreLerp(deltaTime, pNum);

	for (unsigned int i = 0; i < textShip.size(); i++)
	{
		drawChildren(&renderShips[pNum], &lightShips[pNum], textShip[i], true);
	}

	for (unsigned int i = 0; i < UITextShips[pNum].size(); i++)
	{
		UITextShips[pNum][i]->update(deltaTime);
		UIDrawChildren(&UIRenderShips[pNum], UITextShips[pNum][i]);
	}
}

void Game::scoreLerp(float dt, int pNum)
{
	if (players[pNum]->POINT_TOTAL != players[pNum]->LERP_TOTAL)
	{
		float PS = smartScale[pNum].x;
		players[pNum]->LERP_TOTAL = lerp(players[pNum]->LERP_TOTAL, players[pNum]->POINT_TOTAL + 5, 1.f - pow(0.8f, deltaTime * 60.f));
		if (players[pNum]->LERP_TOTAL > players[pNum]->POINT_TOTAL)
			players[pNum]->LERP_TOTAL = players[pNum]->POINT_TOTAL;
		TUI[pNum]->setMessage(std::to_string(players[pNum]->LERP_TOTAL));
		TUI2[pNum]->setMessage(std::to_string(players[pNum]->LERP_TOTAL));

		TUI[pNum]->aS = vec3(lerp(PS, 2.f * PS, 1.f - pow(0.99f, (float)(players[pNum]->POINT_TOTAL - players[pNum]->LERP_TOTAL))));
		TUI2[pNum]->aS = TUI[pNum]->aS;

		TUI[pNum]->setLocalPos(vec3(UIcams[pNum]->m_pOrthoSize.y, UIcams[pNum]->m_pOrthoSize.w, 0) + vec3(-1 - TUI[pNum]->wordLength * 0.5f, -1, 0) * TUI[pNum]->aS);
		TUI2[pNum]->setLocalPos(TUI[pNum]->getLocalPos() + vec3(0.1f, 0.1f, 0) * TUI2[pNum]->aS);
	}
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
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		guiEnabled = !guiEnabled;
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

	vec2 sSize = EPD::screenDims * vec2((float)windowWidth, (float)windowHeight);
	float pseudoAsp = EPD::screenDims.x / EPD::screenDims.y;

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			PlayerCams[i]->perspective(60.f, aspect * pseudoAsp, 1, 1000);
			UIcams[i]->giveNewOrthoRatio(aspect * pseudoAsp);
			PlayerCams[i]->update(0);
			UIcams[i]->update(0);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			TIMER->setLocalPos(vec3(UIcams[i]->m_pOrthoSize.x, UIcams[i]->m_pOrthoSize.w, 0) + vec3(1 + TIMER->wordLength * 0.5f, -1, 0) * smartScale[4]);

			TUI[i]->setLocalPos(vec3(UIcams[i]->m_pOrthoSize.y, UIcams[i]->m_pOrthoSize.w, 0) + vec3(-1 - TUI[i]->wordLength * 0.5f, -1, 0) * smartScale[i]);
			TUI2[i]->setLocalPos(TUI[i]->getLocalPos() + vec3(0.1f, 0.1f, 0) * smartScale[i]);
		}
	}

	TIMER2->setLocalPos(TIMER->getLocalPos() + vec3(0.1f, 0.1f, 0) * smartScale[4]);

	radialHeight = windowHeight / 4;
	bloomHeight = windowHeight / 4;

	//camera.perspective(90.0f, aspect, 0.05f, 1000.0f);
	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	//RES = vec4(sSize.x, sSize.y, 1.f / sSize.x, 1.f / sSize.y);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);
	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(w, h);
	}

	sceneCapture->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);
	defLight->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);
	UI_SCREEN->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);

	//std::cout << "PHASE 1" << std::endl;
	RADIAL_POST_PROC.reshape((unsigned int)sSize.x, (unsigned int)sSize.y);
	//std::cout << "PHASE 2" << std::endl;

	//std::cout << UIcam->getProjection() << std::endl;
	//std::cout << PlayerCam->getProjection() << std::endl;
}

void Game::updateExternals(float dt)
{
	//std::cout << externalUpdateShip.size() << std::endl;

	for (int i = externalUpdateShip.size() - 1; i >= 0; --i)
	{
		if (externalUpdateShip[i]->TT == Transform::TransformType::TYPE_Powerup && externalUpdateShip[i]->destroying)
		{
			for (int p = 0; p < 4; p++)
			{
				for (int j = (int)renderShips[p].size() - 1; j >= 0; --j)
				{
					if (renderShips[p][j] == externalUpdateShip[i])
						renderShips[p].erase(renderShips[p].begin() + j);
				}
			}

			GameObject* _O = nullptr;
			for (int k = (int)theMap->fieldObjects[externalUpdateShip[i]->mapX][externalUpdateShip[i]->mapY].size() - 1; k >= 0; --k)
			{
				_O = theMap->fieldObjects[externalUpdateShip[i]->mapX][externalUpdateShip[i]->mapY][k];
				if (_O == externalUpdateShip[i])
					theMap->removeObj(_O->mapX, _O->mapY, _O);
			}

			//for (int j = (int)updateShip.size() - 1; j >= 0; --j)
			//{
			//	if (updateShip[j] == externalUpdateShip[i])
			//		updateShip.erase(updateShip.begin() + j);
			//}

			rm::destroyObjectINGAME(externalUpdateShip[i]);
			externalUpdateShip.erase(externalUpdateShip.begin() + i);
		}
		else
		{
			protectedEntityShip(&updateShip, externalUpdateShip[i]);
			if (!externalUpdateShip[i]->needsUpdate)
			{
				externalUpdateShip[i]->hasBeenUpdated = false;
				externalUpdateShip.erase(externalUpdateShip.begin() + i);
			}
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
			vec3 curPos = staticCollisionShip[j]->getLocalPos();
			bool desting = staticCollisionShip[j]->destroying;
			dynamicCollisionShip[i]->doCollision(staticCollisionShip[j]);
			if (!desting && staticCollisionShip[j]->destroying)
			{
				//staticCollisionShip[j]->needsUpdate = true;
				//protectedEntityShip(&externalUpdateShip, staticCollisionShip[j]);
				//std::cout << "YES!" << std::endl;
				if (staticCollisionShip[j]->destrPoints > rand() % 200)
				{
					//std::cout << "PUT ONE UP!" << std::endl;
					Powerup* _PUP = rm::getCloneOfPowerup("POWERUP");
					int I = rand() % 3;
					if (I == 0)
					{
						_PUP->setPower(Powerup::pType::MINE);
					}
					else if (I == 1)
					{
						_PUP->setPower(Powerup::pType::HAMMER);
					}
					else if (I == 2)
					{
						_PUP->setPower(Powerup::pType::AXE);
					}
					_PUP->setLocalPos(curPos);
					int xPos = clamp((int)(_PUP->getLocalPos().x / tileSize + 0.5f), 0, 99);
					int yPos = clamp((int)(_PUP->getLocalPos().z / tileSize + 0.5f), 0, 99);
					//std::cout << _PUP->getLocalPos() << std::endl;
					theMap->fieldObjects[xPos][yPos].push_back(_PUP);
				}
			
				if (staticCollisionShip[j]->TT == Transform::TransformType::TYPE_Powerup)
				{
					if (Player* _P = dynamic_cast<Player*>(dynamicCollisionShip[i]))
					{
						if (staticCollisionShip[j]->getChildren().at(0)->getName() == "MINE_MODEL")
						{
							_P->attachWeapon(rm::getWeapon("MINE"));
							//std::cout << "SHIFTED TO MINE" << std::endl;
						}
						else if (staticCollisionShip[j]->getChildren().at(0)->getName() == "HAMMER_MODEL")
						{
							_P->attachWeapon(rm::getWeapon("HAMMER"));
							//std::cout << "SHIFTED TO HAMMER" << std::endl;
						}
						else if (staticCollisionShip[j]->getChildren().at(0)->getName() == "AXE_MODEL")
						{
							_P->attachWeapon(rm::getWeapon("AXE"));
							//std::cout << "SHIFTED TO AXE" << std::endl;
						}
			
						int xPos = clamp((int)(staticCollisionShip[j]->getLocalPos().x / tileSize + 0.5f), 0, 99);
						int yPos = clamp((int)(staticCollisionShip[j]->getLocalPos().z / tileSize + 0.5f), 0, 99);
						theMap->removeObj(xPos, yPos, staticCollisionShip[j]);
					}
				}
			}
		}
	}

	for (int i = 0; i < sA; ++i)
	{
		if (staticCollisionShip[i]->needsUpdate)
		{
			protectedEntityShip(&externalUpdateShip, staticCollisionShip[i]);
			//std::cout << "HAHA!" << std::endl;
		}
	}

	for (int i = 0; i < dA; ++i)
	{
		if (dynamicCollisionShip[i]->needsUpdate)
		{
			protectedEntityShip(&externalUpdateShip, dynamicCollisionShip[i]);
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

void Game::protectedWeaponShip(Weapon * _W)
{
	if (weaponShip.size() > 0)
		protectedAddWeapon(_W, 0, weaponShip.size() - 1);
	else
		weaponShip.push_back(_W);
}

void Game::protectedAddWeapon(Weapon* _W, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if (weaponShip[mid] == _W)
	{
		//allGameObjects.insert(allGameObjects.begin() + mid, ELEM);
	}
	else if (back - front <= 1)
	{
		if (weaponShip[front] > _W)
			weaponShip.insert(weaponShip.begin() + front, _W);
		else if (weaponShip[back] > _W)
			weaponShip.insert(weaponShip.begin() + back, _W);
		else
			weaponShip.insert(weaponShip.begin() + back + 1, _W);
		//if (_W == players[0])
		//	std::cout << "PLAYER SHIP RECEIVED!" << std::endl;
		_W->hasBeenUpdated = true;
	}
	else if (weaponShip[mid] > _W)
	{
		protectedAddWeapon(_W, front, mid);
	}
	else
	{
		protectedAddWeapon(_W, mid, back);
	}
}

void Game::generateATTACK(Player * P)
{
	//std::cout << P->getWeapon()->getName() << std::endl;
	if (P->getWeapon())
	{
		//std::cout << "ATTTAAAAAAAACK" << std::endl;
		if (P->getWeapon()->TT == Transform::TransformType::TYPE_Mine)
		{
			Weapon* W;
			W = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W->ownedPlayer = P->playerNumber;
			W->worldLocation = P->getLocalToWorld();

			protectedWeaponShip(W);
		}
		else if (P->getWeapon()->TT == Transform::TransformType::TYPE_Hammer)
		{
			Weapon* W, *W2;
			W = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W2 = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W->ownedPlayer = P->playerNumber;
			W2->ownedPlayer = P->playerNumber;
			W->worldLocation = P->getLocalToWorld();
			W2->worldLocation = P->getLocalToWorld() * mat4::rotatey(degrees(180.f));

			protectedWeaponShip(W);
			protectedWeaponShip(W2);
		}
		if (P->getWeapon()->TT == Transform::TransformType::TYPE_Axe)
		{
			//std::cout << "READY!?" << std::endl;
			Weapon* W, *W2, *W3;
			W = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W->ownedPlayer = P->playerNumber;
			W->worldLocation = P->getLocalToWorld();

			W2 = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W2->ownedPlayer = P->playerNumber;
			W2->worldLocation = P->getLocalToWorld() * mat4::rotatey(degrees(15.f));

			W3 = ResourceManager::getCloneOfWeapon(P->getWeapon()->getName());
			W3->ownedPlayer = P->playerNumber;
			W3->worldLocation = P->getLocalToWorld() * mat4::rotatey(degrees(-15.f));

			protectedWeaponShip(W);
			protectedWeaponShip(W2);
			protectedWeaponShip(W3);
		}
	}
}

void Game::updateAttacks(float dt)
{
	for (int i = weaponShip.size() - 1; i >= 0; --i)
	{
		weaponShip[i]->update(dt);
		if (weaponShip[i]->flingHitbox)
		{
			attackHIT(i);
			weaponShip[i]->flingHitbox = 0;
		}
		if (weaponShip[i]->timeToDie)
		{
			ResourceManager::destroyObjectINGAME(weaponShip[i]);
			weaponShip.erase(weaponShip.begin() + i);
			//std::cout << "SHE GONE" << std::endl;
		}
		else
		{
			drawChildren(&renderShips[weaponShip[i]->ownedPlayer], &lightShips[weaponShip[i]->ownedPlayer], weaponShip[i], true);
		}
	}
}

void Game::attackHIT(unsigned int index)
{
	//std::cout << "BOOM!" << std::endl;
	float SCALE = 9.f;
	int IS = (int)(SCALE / 6.f) + 1;
	
	//vec2 pW = (weaponShip[index]->getLocalToWorld().translation().xz + (mat3(weaponShip[index]->getLocalToWorld()) * weaponShip[index]->hitboxOffset).xz) / tileSize + vec2(0.5);
	//mat4 weapLoc = weaponShip[index]->getLocalToWorld() * mat4::rotatey(degrees(weaponShip[index]->getLocalEuler().y)) * mat4(mat3::identity(), weaponShip[index]->hitboxOffset);
	vec2 pW = (weaponShip[index]->getLocalToWorld() * mat4::rotatey(degrees(weaponShip[index]->getLocalEuler().y))
		* vec4(weaponShip[index]->hitboxOffset, 1.f)).xz  / tileSize + vec2(0.5);
	//std::cout << players[0]->getWorldPos() << std::endl;
	//std::cout << (weaponShip[index]->getLocalToWorld() * mat4::rotatey(degrees(weaponShip[index]->getLocalEuler().y))
	//	* vec4(weaponShip[index]->hitboxOffset, 1.f)).xz << std::endl;
	int pWX = (int)pW.x;
	int pWY = (int)pW.y;

	for (int i = pWX - IS; i <= pWX + IS; ++i)
	{
		for (int j = pWY - IS; j <= pWY + IS; ++j)
		{
			if (i >= 0 && i < 100 && j >= 0 && j < 100)
			{
				for (unsigned int k = 0; k < theMap->fieldObjects[i][j].size(); k++)
				{
					GameObject* _GO = theMap->fieldObjects[i][j][k];
					//std::cout << i << ", " << j << std::endl;

					if (_GO && !_GO->destroying && !_GO->destroyed)
					{
						vec3 curPos = _GO->getLocalPos();
						if (weaponShip[index]->tailoredCollision(_GO))
						{
							//_GO->getPhysicsBody()->getHB()->enabled = false;
							//_GO->destroying = true;
							//players[weaponShip[index]->ownedPlayer]->POINT_TOTAL += _GO->destrPoints;
							//std::cout << players[weaponShip[index]->ownedPlayer]->POINT_TOTAL << std::endl;
							_GO->needsUpdate = true;
							protectedEntityShip(&externalUpdateShip, _GO);

							if (_GO->destrPoints > rand() % 600)
							{
								//std::cout << "PUT ONE UP!" << std::endl;
								Powerup* _PUP = rm::getCloneOfPowerup("POWERUP");
								int I = rand() % 3;
								if (I == 0)
								{
									_PUP->setPower(Powerup::pType::MINE);
								}
								else if (I == 1)
								{
									_PUP->setPower(Powerup::pType::HAMMER);
								}
								else if (I == 2)
								{
									_PUP->setPower(Powerup::pType::AXE);
								}
								_PUP->setLocalPos(curPos);
								int xPos = clamp((int)(_PUP->getLocalPos().x / tileSize + 0.5f), 0, 99);
								int yPos = clamp((int)(_PUP->getLocalPos().z / tileSize + 0.5f), 0, 99);
								//std::cout << _PUP->getLocalPos() << std::endl;
								theMap->fieldObjects[xPos][yPos].push_back(_PUP);
							}

							//std::cout << "GOTTEM" << std::endl;
						}
					}
					else if (!_GO)
					{
						theMap->removeObj(i, j, _GO);
					}
				}
			}
		}
	}
}

void Game::allSetup()
{
	setBaseAndBoundaries();
	setShaders();
	setFramebuffers();
	setCamerasAndPlayers();
	generateMap();
}

void Game::setBaseAndBoundaries()
{
	UPPER_WALL = rm::getCloneOfBoundary("TOP_AND_BOT_COLLIDER");
	LOWER_WALL = rm::getCloneOfBoundary("TOP_AND_BOT_COLLIDER");
	LEFT_WALL = rm::getCloneOfBoundary("LEFT_AND_RIGHT_COLLIDER");
	RIGHT_WALL = rm::getCloneOfBoundary("LEFT_AND_RIGHT_COLLIDER");

	UPPER_WALL->setLocalPos(vec3(297.f, 0, -9.f));
	LOWER_WALL->setLocalPos(vec3(297.f, 0, 603.f));
	LEFT_WALL->setLocalPos(vec3(-9.f, 0, 297.f));
	RIGHT_WALL->setLocalPos(vec3(603.f, 0, 297.f));
	for (int i = 0; i < 4; i++)
	{
		SUNS[i] = nullptr;
		if (EPD::playerActive[i])
		{
			SUNS[i] = rm::getLight("SUNLIGHT");
		}
	}

	BASE_PLATE_SAND = rm::getBasePlate("FIELD_SAND");
	BASE_PLATE_T = rm::getBasePlate("FIELD_ROAD_T");
	BASE_PLATE_ROAD = rm::getBasePlate("FIELD_ROAD_STRAIGHT");
	BASE_PLATE_CORNER = rm::getBasePlate("FIELD_ROAD_CORNER");
	BASE_PLATE_4WAY = rm::getBasePlate("FIELD_ROAD_4WAY");
	BASE_PLATE_GRASS = rm::getBasePlate("FIELD_GRASS");
	BASE_PLATE_DIRT = rm::getBasePlate("FIELD_DIRT");
	BASE_PLATE_CONCRETE = rm::getBasePlate("FIELD_CONCRETE");
}

void Game::setShaders()
{
	//PassThrough = getShader("MAIN_RENDER");
}

void Game::setFramebuffers()
{
	//std::cout << "HERE?" << std::endl;

	vec2 sSize = EPD::screenDims * vec2((float)windowWidth, (float)windowHeight);
	RADIAL_POST_PROC.reshape((unsigned int)sSize.x, (unsigned int)sSize.y);

	tSwitch = rm::getTexture("SCENE_GAME_TRANSITION");
	tFade = rm::getTexture("defaultBlack");

	sceneCapture->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);
	defLight->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);
	UI_SCREEN->reshape((unsigned int)sSize.x, (unsigned int)sSize.y);

	//std::cout << "HERE!" << std::endl;
}

void Game::setCamerasAndPlayers()
{
	float pseudoAsp = EPD::screenDims.x / EPD::screenDims.y;

	//std::cout << pseudoAsp << std::endl;

	for (int i = 0; i < 4; i++)
	{
		PlayerCams[i] = nullptr;
		UIcams[i] = nullptr;
		if (EPD::playerActive[i])
		{
			//std::cout << "THEN HERE" << std::endl;
			PlayerCams[i] = rm::getCloneOfCamera("PLAYER_CAM");
			PlayerCams[i]->giveNewPersRatio(aspect * pseudoAsp);
			//std::cout << PlayerCams[i]->getProjection() << std::endl;
			//std::cout << "AHA!" << std::endl;
			PlayerCams[i]->setRenderList(renderShips[i]);

			//std::cout << "OR HERE" << std::endl;

			UIcams[i] = rm::getCloneOfCamera("UI_CAM");
			UIcams[i]->giveNewOrthoRatio(aspect * pseudoAsp);
			UIcams[i]->setRenderList(UIRenderShips[i]);
		}
	}
}

void Game::generateMap()
{
	//std::cout << "MAP GENERATIN'" << std::endl;

	std::string MAP_DIRECTORY = "TEST_MAP.txt";
	theMap = new Field(MAP_DIRECTORY);

	players = theMap->players;
	
	spawnPoints = theMap->spawnPoints;
	gridPoints = theMap->gridPoints;

	float camHeight;
	if (EPD::numActive == 2)
	{
		camHeight = 27.f;
	}
	else
	{
		camHeight = 20.f;
	}

	TIMER = rm::getCloneOfText("UISpaces");
	TIMER->baseColor = vec3(0.f);
	TIMER->setMessage("3:00");
	
	switch (EPD::numActive)
	{
	case 0:
		std::cout << "Damn son, how the fuck did you play a game with no players?" << std::endl;
		break;
	case 1:
		TIMER->aS = vec3(6.f);
		smartScale[4] = TIMER->aS;
		break;
	case 2:
		TIMER->aS = vec3(6.f) * 0.7f;
		smartScale[4] = TIMER->aS;
		break;
	case 3:
		TIMER->aS = vec3(6.f) * 1.2f;
		smartScale[4] = TIMER->aS;
		break;
	case 4:
		TIMER->aS = vec3(6.f) * 1.2f;
		smartScale[4] = TIMER->aS;
		break;
	}

	//for (unsigned int i = 0; i < spawnPoints.size(); i++)
	//{
	//	std::cout << spawnPoints[i] << std::endl;
	//	std::cout << gridPoints[i] << std::endl;
	//}

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			TIMER->setLocalPos(vec3(UIcams[i]->m_pOrthoSize.x, UIcams[i]->m_pOrthoSize.w, 0) + vec3(1 + TIMER->wordLength * 0.5f, -1, 0) * smartScale[4]);
		}
	}

	TIMER2 = rm::getCloneOfText("UISpaces");
	TIMER2->baseColor = vec3(1.f);
	TIMER2->setMessage("3:00");
	//TIMER2->setLocalPos(vec3(-35, 40, 0));
	TIMER2->setLocalPos(TIMER->getLocalPos() + vec3(0.1f, 0.1f, 0) * smartScale[4]);
	//TIMER2->aS = vec3(6.f);
	TIMER2->aS = TIMER->aS;

	for (int i = 0; i < 4; i++)
	{
		players[i] = nullptr;
		if (EPD::playerActive[i])
		{
			if (EPD::playerVehicleChoice[i] == 0)
			{ 
				players[i] = rm::getCloneOfPlayer("PLAYER_TRUCK");
			}
			else if (EPD::playerVehicleChoice[i] == 1)
			{
				players[i] = rm::getCloneOfPlayer("PLAYER_TANK");
			}
			else if (EPD::playerVehicleChoice[i] == 2)
			{
				players[i] = rm::getCloneOfPlayer("PLAYER_BULLDOZER");
			}
			else if (EPD::playerVehicleChoice[i] == 3)
			{
				players[i] = rm::getCloneOfPlayer("PLAYER_WRECKINGBALL");
			}

			Player* PL = players[i];

			PL->playerNumber = i;
			int randomSpawn = rand() % spawnPoints.size();

			PL->setLocalPos(spawnPoints[randomSpawn]);
			PL->setLocalRot(vec3(0, gridPoints[randomSpawn].z, 0));
			PL->setScale(vec3(1.f));
			PL->mapX = (int)gridPoints[randomSpawn].x;
			PL->mapY = (int)gridPoints[randomSpawn].y;
			PL->setInitials(PL->getLocalPos(), PL->getLocalEuler(), PL->getLocalScale());
			PL->setBob(((float)(rand() % 629)) * 0.01f + 6.28f);
			PL->hasInitial = true;

			spawnPoints.erase(spawnPoints.begin() + randomSpawn);
			gridPoints.erase(gridPoints.begin() + randomSpawn);

			PL->addChild(PlayerCams[i]);
			PL->attachWeapon(ResourceManager::getWeapon("MINE"));

			theMap->fieldObjects[PL->mapX][PL->mapY].push_back(PL);

			//std::cout << PL->getLocalPos() << std::endl;

			PlayerCams[i]->setLocalPos(vec3(0, camHeight, camHeight / (float)sqrt(3)));
			PlayerCams[i]->setLocalRot(vec3(-60.f, 0, 0));

			if (PL->getName() == "PLAYER_TRUCK")
				PL->playerInit(Player::PLAYER_TYPE::TRUCK);
			else if (PL->getName() == "PLAYER_TANK")
				PL->playerInit(Player::PLAYER_TYPE::TANK);
			else if (PL->getName() == "PLAYER_BULLDOZER")
				PL->playerInit(Player::PLAYER_TYPE::BULLDOZER);
			else if (PL->getName() == "PLAYER_WRECKINGBALL")
				PL->playerInit(Player::PLAYER_TYPE::WRECKING_BALL);


			TUI[i] = rm::getCloneOfText("UISpaces");
			//std::cout << TUI[i]->getMaterial()->getName() << std::endl;
			TUI[i]->baseColor = vec3(0.0f);
			TUI[i]->setMessage("0");
			
			switch (EPD::numActive)
			{
			case 0:
				std::cout << "No, seriously how THE FUCK?" << std::endl;
				break;
			case 1:
				TUI[i]->aS = vec3(6.f);
				smartScale[i] = TUI[i]->aS;
				break;
			case 2:
				TUI[i]->aS = vec3(6.f) * 0.7f;
				smartScale[i] = TUI[i]->aS;
				break;
			case 3:
				TUI[i]->aS = vec3(6.f) * 0.5f;
				smartScale[i] = TUI[i]->aS;
				break;
			case 4:
				TUI[i]->aS = vec3(6.f) * 0.5f;
				smartScale[i] = TUI[i]->aS;
				break;
			}

			TUI[i]->setLocalPos(vec3(UIcams[i]->m_pOrthoSize.y, UIcams[i]->m_pOrthoSize.w, 0) + vec3(-1 - TUI[i]->wordLength * 0.5f, -1, 0) * smartScale[i]);

			//TUI[i]->setLocalPos(vec3(39, 39, 0));
			//TUI[i]->aS = vec3(4.0f);

			TUI2[i] = rm::getCloneOfText("UISpaces");
			TUI2[i]->baseColor = vec3(1.0f);
			TUI2[i]->setMessage("0");
			TUI2[i]->setLocalPos(TUI[i]->getLocalPos() + vec3(0.1f, 0.1f, 0) * smartScale[i]);
			TUI2[i]->aS = TUI[i]->aS;

			UITextShips[i].push_back(TUI[i]);
			UITextShips[i].push_back(TUI2[i]);
			UITextShips[i].push_back(TIMER);
			UITextShips[i].push_back(TIMER2);
		}
	}

	spawnPoints = theMap->spawnPoints;
	gridPoints = theMap->gridPoints;

	for (Transform* object : ResourceManager::Transforms)
	{
		object->update(0);
	}

	for (Transform* object : ResourceManager::TransformsINGAME)
	{
		object->update(0);
	}
}

void Game::performUpdates(float dt)
{
	vec2 pCur, pNew;
	int PX = 0;
	int PY = 0;
	int NX = 0;
	int NY = 0;
	for (GameObject* object : updateShip)
	{

		bool setNew = false;
		bool destCheck = object->destroyed;
		switch (object->TT)
		{
		case Transform::TransformType::TYPE_Player:
			//std::cout << "PLAYER_UPDATE!" << std::endl;
		case Transform::TransformType::TYPE_Destructable:
			pCur = object->getLocalPos().xz / tileSize + vec2(0.5);
			setNew = true;
		default:
			object->update(dt);
			break;
		}
		

		if (object->destroyed && !destCheck)
		{
			//std::cout << "HEGON" << std::endl;
			players[object->playerResponsible]->POINT_TOTAL += object->destrPoints;
			Text* jext = rm::getCloneOfText("TextSpaces");
			jext->baseColor = vec3(1.f);
			jext->setMessage(std::to_string(object->destrPoints));
			jext->setLocalPos((object->getLocalToWorld() * object->getDestrMat()).translation());
			pTotals.push_back(jext);
			pFloats.push_back(pMax);
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
					//std::cout << object->mapX << ", " << object->mapY << std::endl;
					theMap->removeObj(object->mapX, object->mapY, object);
					theMap->fieldObjects[NX][NY].push_back(object);
					object->mapX = NX;
					object->mapY = NY;
				}
		}
	}
}

void Game::updateSingle(float dt, GameObject* _T)
{
	vec2 pCur, pNew;

	int PX = 0;
	int PY = 0;
	int NX = 0;
	int NY = 0;

	bool setNew = false;
	switch (_T->TT)
	{
	case Transform::TransformType::TYPE_Player:
		//std::cout << _T->mapX << ", " << _T->mapY << std::endl;
	case Transform::TransformType::TYPE_Destructable:
		pCur = _T->getLocalPos().xz / tileSize + vec2(0.5);
		setNew = true;
	default:
		_T->update(dt);
		break;
	}

	if (setNew)
	{
		pNew = _T->getLocalPos().xz / tileSize + vec2(0.5);

		PX = (int)pCur.x;
		PY = (int)pCur.y;
		NX = (int)pNew.x;
		NY = (int)pNew.y;

		//if (_T->TT == Transform::TransformType::TYPE_Player)
		//	std::cout << PX << ", " << PY << "||" << NX << ", " << NY << std::endl;
		if (NX >= 0 && NX < 100 && NY >= 0 && NY < 100)
		{
			//std::cout << NX << ", " << NY << std::endl;
			theMap->removeObj(_T->mapX, _T->mapY, _T);
			theMap->fieldObjects[NX][NY].push_back(_T);
			_T->mapX = NX;
			_T->mapY = NY;
		}
	}
}

void Game::uniqueKeyPresses()
{
	if (keysDown['p'] && !backCheckKeysDown['p'])
	{
		paused = true;
	}
	if (keysDown['m'] && !backCheckKeysDown['m'])
	{
		resetMap();
	}

	if (keysDown['/'] && !backCheckKeysDown['/'])
	{
		if (_GS == GS_RUNNING)
		{
			_GS = GS_PAUSED;
		}
		else if (_GS == GS_PAUSED)
		{
			_GS = GS_RUNNING;
		}
	}
	if (keysDown['\\'])
	{
		minutes = 0;
		seconds = 0;
	}
}

void Game::resetMap()
{
	//std::cout << "HAPPENED" << std::endl;
	std::vector<Transform*> EMPT;
	for (unsigned int i = 0; i < rm::CamerasINGAME.size(); i++)
	{
		rm::CamerasINGAME[i]->setRenderList(EMPT);
	}
	for (int i = 0; i < 4; i++)
	{
		renderShips[i].clear();
	}
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			unsigned int OBS = theMap->fieldObjects[i][j].size();
			for (int k = (int)OBS - 1; k >= 0; --k)
			{
				if (theMap->fieldObjects[i][j][k]->TT == Transform::TransformType::TYPE_Powerup)
				{
					GameObject* _O = theMap->fieldObjects[i][j][k];
					theMap->removeObj(i, j, _O);
					rm::destroyObjectINGAME(_O);
					//std::cout << "BEGONE THOT" << std::endl;
				}
				else if (theMap->fieldObjects[i][j][k]->hasInitial)
				{
					//theMap->fieldObjects[i][j][k]->resetToInitials();
					////theMap->fieldObjects[i][j][k]->update(0);
					//updateSingle(0, theMap->fieldObjects[i][j][k]);
					RE_SPAWN.push_back(theMap->fieldObjects[i][j][k]);
				}
			}
		}
	}

	minutes = 3;
	seconds = 0;

	for (int i = RE_SPAWN.size() - 1; i >= 0; --i)
	{
		RE_SPAWN[i]->resetToInitials();
		updateSingle(0, RE_SPAWN[i]);
	}

	RE_SPAWN.clear();

	//std::cout << "RESET: " << players[0]->getLocalPos() << std::endl;
}
