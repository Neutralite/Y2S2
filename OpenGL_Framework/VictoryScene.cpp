#include "VictoryScene.h"
#include "ExternalPlayerData.h"

//#define _CRT_SECURE_NO_WARNINGS

VictoryScene::VictoryScene()
{
	updateTimer = new Timer();
}

VictoryScene::~VictoryScene()
{
	delete updateTimer;
}

void VictoryScene::initializeGame()
{
	readyToTerminate = false;
	TotalTime = 0.f;
	tTime = 1.0001f;
	TotalGameTime = 0.f;
	holdOff = 0.f;
	sinWaveTime = 0.f;
	fadeOutTime = 0.f;
	justBecauseItFeelsNice = 0.f;

	L1 = 'A';
	L2 = 'A';
	L3 = 'A';

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	playerModels.clear();
	scoreLerp.clear();
	playerScoreBars.clear();
	playerScores.clear();
	playerScoresBACK.clear();
	playerNames.clear();
	editSlot.clear();

	preScores.clear();
	preNames.clear();
	preText.clear();

	//EPD::playerActive[0] = true;
	//EPD::playerScores[0] = 10000;
	//EPD::playerVehicleChoice[0] = 0;


	playerModels.resize(4);
	scoreLerp.resize(4);
	playerScoreBars.resize(4);
	playerScores.resize(4);
	playerScoresBACK.resize(4);
	playerNames.resize(4);
	editSlot.resize(4);

	preScores.resize(20);
	preNames.resize(20);
	preText.resize(20);

	backCheckTriggers.clear();
	backCheckButton.clear();
	backCheckSticks.clear();

	backCheckTriggers.resize(8);
	backCheckButton.resize(8);
	backCheckSticks.resize(8);

	setUpVictory();

	victoryCam->update(0);
	victoryCam->sendUBO();

	updateTimer->tick();
	deltaTime = updateTimer->getElapsedTimeSeconds();

	for (unsigned int i = 0; i < 256; i++)
	{
		setKeysDown(false, (unsigned char)i);
	}

	maxScore = 0;
	enteringPlayer = 0;
	letterSel = 0;
	runThrough = false;

	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i] && EPD::playerScores[i] > maxScore)
			maxScore = (int)EPD::playerScores[i];
	}

	_GS = GS_BEGINNING;
}

void VictoryScene::update()
{

	lightShip.clear();
	renderShip.clear();
	UIRenderShip.clear();
	updateShip.clear();

	updateTimer->tick();
	_INPUT.update();

	for (int i = 0; i < 4; i++)
	{
		if (_INPUT.controllerConnected(i))
			controllers[i] = _INPUT.getController(i);
		else
			controllers[i] = nullptr;
	}

	deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	switch (_GS)
	{
	case GS_BEGINNING:
		beginningUpdate(deltaTime);
		break;
	case GS_STARTING:
		startingUpdate(deltaTime);
		break;
	case GS_ENTER:
		enterUpdate(deltaTime);
	case GS_RUNNING:
		runningUpdate(deltaTime);
		break;
	case GS_ENDED:
		endedUpdate(deltaTime);
		break;
	case GS_EXITING:
		exitingUpdate(deltaTime);
		break;
	}

	frameTimeSamples[frameTimeCurrSample] = min(deltaTime, 0.1f);
	frameTimeCurrSample = (frameTimeCurrSample + 1) % frameTimeNumSamples;
	frameTimeSamples[frameTimeCurrSample] = 1.0f;
}

void VictoryScene::draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();

	victoryCam->sendUBO();
	victoryCam->setRenderList(renderShip);
	victoryCam->cull();
	victoryCam->render();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	COMIC_EXECUTION->bind();
	sceneCapture->bindColorAsTexture(0, 0);
	sceneCapture->bindColorAsTexture(1, 1);
	sceneCapture->bindColorAsTexture(2, 2);
	sceneCapture->bindColorAsTexture(3, 4);
	sceneCapture->bindColorAsTexture(4, 4);
	sceneCapture->bindDepthAsTexture(5);
	tRamp->bind(31);

	COMIC_EXECUTION->sendUniform("uModel", SUN->getLocalToWorld(), false);
	defLight->renderToFSQ();

	for (Light* LIT : lightShip)
	{
		ShaderProgram* USING = nullptr;
		LIT->update(0);

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

		for (Transform* C : LIT->getChildren())
		{
			if (C->TT == Transform::TransformType::TYPE_Camera)
			{
				C->getFrameBuffer()->unbindTexture(4);
			}
		}
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

	COMBINED_DRAW->bind();
	COMBINED_DRAW->sendUniform("texRot", 0.4f);
	COMBINED_DRAW->sendUniform("ASPECT", vec2((float)windowWidth, (float)windowHeight));

	transition->renderToFSQ();
	COMBINED_DRAW->unbind();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	tDiffuse->unbind(31);
	overlay->unbind(30);
	defLight->unbindTexture(6);
	defLight->unbindTexture(5);
	defLight->unbindTexture(4);
	sceneCapture->unbindTexture(3);
	sceneCapture->unbindTexture(2);
	sceneCapture->unbindTexture(1);
	sceneCapture->unbindTexture(0);

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

	if (guiEnabled)
		GUI();

	glutSwapBuffers();
}

void VictoryScene::GUI()
{
	UI::Start(windowWidth, windowHeight);
	// Framerate Visualizer
	ImGui::Begin("Framerate");
	{
		float averageFramerate = 0.0f;
		for (int i = 0; i < frameTimeNumSamples; ++i)
			averageFramerate += frameTimeSamples[i];
		averageFramerate = 1.0f / ((averageFramerate - 1.0f) / (frameTimeNumSamples - 1));
		std::string framerate = "Framerate: " + std::to_string(averageFramerate);

		ImGui::PlotHistogram("", frameTimeSamples, frameTimeNumSamples, 0, framerate.c_str(), 0.0f, 0.1f, ImVec2(frameTimeNumSamples, 60));
	}

	vec3 cPos = victoryCam->getLocalPos();
	ImGui::SliderFloat3("CAMERA_POSITION", &cPos.x, -10, 10, "%.5f");
	victoryCam->setLocalPos(cPos);

	ImGui::End();

	UI::End();
}

void VictoryScene::releaseScene()
{
	rm::order66();
}

void VictoryScene::beginningUpdate(float dt)
{
	tTime -= deltaTime * 0.8f;
	//std::cout << tTime << std::endl;
	if (tTime < 0.f)
	{
		justBecauseItFeelsNice += deltaTime * 1.3f;
		if (justBecauseItFeelsNice > 1.f)
		{
			justBecauseItFeelsNice = 1.f;
			_GS = GS_STARTING;
		}
		tTime = 0.f;
	}

	victoryCam->update(deltaTime);

	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	protectedEntityShip(&updateShip, (GameObject*) backdrop);

	drawChildren(&renderShip, &lightShip, backdrop, false);
	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			drawChildren(&renderShip, &lightShip, playerModels[i], false);
			drawChildren(&renderShip, &lightShip, playerScoreBars[i], false);
			drawChildren(&renderShip, &lightShip, playerScores[i], false);
		}
	}

	protectedEntityShip(&lightShip, SUN);

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	performUpdates(deltaTime);
}

void VictoryScene::startingUpdate(float dt)
{
	victoryCam->update(deltaTime);

	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	protectedEntityShip(&updateShip, (GameObject*)backdrop);

	drawChildren(&renderShip, &lightShip, backdrop, false);

	bool stillGoin = false;
	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			drawChildren(&renderShip, &lightShip, playerModels[i], false);
			drawChildren(&renderShip, &lightShip, playerScoreBars[i], false);
			drawChildren(&renderShip, &lightShip, playerScores[i], false);

			protectedEntityShip(&updateShip, (GameObject*)playerScoreBars[i]);
			protectedEntityShip(&updateShip, (GameObject*)playerScores[i]);
			protectedEntityShip(&updateShip, (GameObject*)playerModels[i]);

			scoreLerp[i] = (scoreLerp[i] - EPD::playerScores[i] - 5) * pow(0.9f, 60.f * dt) + EPD::playerScores[i] + 5;
			scoreLerp[i] = min(scoreLerp[i], EPD::playerScores[i]);
			if (scoreLerp[i] != EPD::playerScores[i])
			{
				stillGoin = true;
			}

			playerScores[i]->setMessage(std::to_string((int)scoreLerp[i]));
			playerScoresBACK[i]->setMessage(std::to_string((int)scoreLerp[i]));
			playerScores[i]->baseColor = vec3(1.f);
			playerScoresBACK[i]->baseColor = vec3(1.f);

			for (unsigned int j = 0; j < playerScores[i]->messageSize(); j++)
			{
				playerScores[i]->colorShift[j] = playerScoreBars[i]->objectColor;
				playerScoresBACK[i]->colorShift[j] = vec3(1.f);
				playerScoresBACK[i]->tS[j] = vec3(1.1f);
			}

			if (EPD::playerScores[i] > 0)
			{
				playerScoreBars[i]->setScale(vec3(1.f, 4.f * ((float)scoreLerp[i]) / (float)maxScore, 1.f));
				playerModels[i]->setLocalPos(vec3(-6.f + 4.f * i, -4.f + 2.f * playerScoreBars[i]->getLocalScale().y, 0.f));
				playerScores[i]->setLocalPos(playerModels[i]->getWorldPos() + vec3(0.f, 5.f, 4.f));
			}
		}
	}

	if (!stillGoin)
	{
		holdOff += deltaTime;
		if (holdOff > 1.f)
		{
			holdOff = 1.f;
			_GS = GS_ENTER;
		}
	}

	protectedEntityShip(&lightShip, SUN);

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	performUpdates(deltaTime);
}

void VictoryScene::runningUpdate(float dt)
{
	victoryCam->update(deltaTime);

	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	protectedEntityShip(&updateShip, (GameObject*)backdrop);
	protectedEntityShip(&updateShip, (GameObject*)U_A);
	protectedEntityShip(&updateShip, (GameObject*)D_A);

	drawChildren(&renderShip, &lightShip, backdrop, false);
	drawChildren(&renderShip, &lightShip, confirmation, false);
	if (leaderboarders > 0)
		drawChildren(&renderShip, &lightShip, nameInitials, false);
	if (letterSel < 3)
	{
		if (enteringPlayer < 4)
		{
			drawChildren(&renderShip, &lightShip, U_A, false);
			drawChildren(&renderShip, &lightShip, D_A, false);
		}
		for (unsigned int i = 0; i < confirmation->messageSize(); i++)
		{
			confirmation->posOffset[i] = vec3(0, 0, 0);
		}
	}
	else
	{
		for (unsigned int i = 0; i < confirmation->messageSize(); i++)
		{
			confirmation->posOffset[i] = vec3(0, pow(sin(TotalGameTime * 2.f + 0.6f * i), 6), 0);
		}
	}

	for (int i = 0; i < 20; i++)
	{
		drawChildren(&renderShip, &lightShip, preText[i], false);
	}

	while (enteringPlayer < 4 && (!EPD::playerActive[enteringPlayer] || editSlot[enteringPlayer] < 0))
	{
		enteringPlayer++;
	}
	
	bool newEnter = false;
	if (enteringPlayer < 4)
	{
		runThrough = true;
		nameInitials->setMessage("P" + std::to_string(enteringPlayer + 1) + ": " + L1 + "" + L2 + "" + L3);
		nameInitials->baseColor = vec3(1.f);

		U_A->objectColor = playerScoreBars[enteringPlayer]->objectColor;
		U_A->setScale(max(vec3(0.5f), U_A->getLocalScale() * pow(0.92f, 60.f * deltaTime)));
		D_A->objectColor = playerScoreBars[enteringPlayer]->objectColor;
		D_A->setScale(max(vec3(0.5f), D_A->getLocalScale() * pow(0.92f, 60.f * deltaTime)));

		for (unsigned int i = 0; i < nameInitials->messageSize(); i++)
		{
			nameInitials->colorShift[i] = playerScoreBars[enteringPlayer]->objectColor;
		}

		for (unsigned int i = 0; i < preText[editSlot[enteringPlayer]]->messageSize(); i++)
		{
			preText[editSlot[enteringPlayer]]->colorShift[i] = playerScoreBars[enteringPlayer]->objectColor;
		}

		if (letterSel < 3)
		{
			vec3 hAlf = (nameInitials->individualPos[letterSel + 4]) * nameInitials->getLocalScale();
			U_A->setLocalPos(vec3(0, 0.7f, 0) + hAlf + nameInitials->getLocalPos());
			D_A->setLocalPos(vec3(0, -0.7f, 0) + hAlf + nameInitials->getLocalPos());
		}

		if (keysDown['w'] && !backCheckKeysDown['w'])
		{
			D_A->setScale(0.8f);
			switch (letterSel)
			{
			case 0:
				L1--;
				if (L1 < 'A')
					L1 = 'Z';
				break;
			case 1:
				L2--;
				if (L2 < 'A')
					L2 = 'Z';
				break;
			case 2:
				L3--;
				if (L3 < 'A')
					L3 = 'Z';
				break;
			}
		}
		if (keysDown['s'] && !backCheckKeysDown['s'])
		{
			U_A->setScale(0.8f);
			switch (letterSel)
			{
			case 0:
				L1++;
				if (L1 > 'Z')
					L1 = 'A';
				break;
			case 1:
				L2++;
				if (L2 > 'Z')
					L2 = 'A';
				break;
			case 2:
				L3++;
				if (L3 > 'Z')
					L3 = 'A';
				break;
			}
		}
		if (keysDown[' '] && !backCheckKeysDown[' '])
		{
			if (letterSel < 3)
				letterSel++;
			else
			{
				preNames[editSlot[enteringPlayer]].at(0) = L1;
				preNames[editSlot[enteringPlayer]].at(1) = L2;
				preNames[editSlot[enteringPlayer]].at(2) = L3;
				
				preText[editSlot[enteringPlayer]]->setMessage(std::to_string(editSlot[enteringPlayer] + 1) + ". " + preNames[editSlot[enteringPlayer]] + " ======= " + std::to_string((int)EPD::playerScores[enteringPlayer]));
				preText[editSlot[enteringPlayer]]->baseColor = vec3(1.f);
				for (unsigned int k = 0; k < preText[editSlot[enteringPlayer]]->messageSize(); k++)
				{
					preText[editSlot[enteringPlayer]]->colorShift[k] = playerScoreBars[enteringPlayer]->objectColor;
				}

				newEnter = true;
			}
		}
		if (keysDown['q'] && !backCheckKeysDown['q'])
		{
			if (letterSel > 0)
				letterSel--;
		}

		if (controllers[enteringPlayer])
		{
			controllers[enteringPlayer]->getSticks(&playerInput[2 * enteringPlayer], &playerInput[2 * enteringPlayer + 1]);
			controllers[enteringPlayer]->getTriggers(&playerTriggers[enteringPlayer]);

			if (playerInput[2 * enteringPlayer].y > 0.5f && !(backCheckSticks[2 * enteringPlayer].y > 0.5f))
			{
				D_A->setScale(0.8f);
				switch (letterSel)
				{
				case 0:
					L1--;
					if (L1 < 'A')
						L1 = 'Z';
					break;
				case 1:
					L2--;
					if (L2 < 'A')
						L2 = 'Z';
					break;
				case 2:
					L3--;
					if (L3 < 'A')
						L3 = 'Z';
					break;
				}
			}
			if (playerInput[2 * enteringPlayer].y < -0.5f && !(backCheckSticks[2 * enteringPlayer].y < -0.5f))
			{
				U_A->setScale(0.8f);
				switch (letterSel)
				{
				case 0:
					L1++;
					if (L1 > 'Z')
						L1 = 'A';
					break;
				case 1:
					L2++;
					if (L2 > 'Z')
						L2 = 'A';
					break;
				case 2:
					L3++;
					if (L3 > 'Z')
						L3 = 'A';
					break;
				}
			}

			if (controllers[enteringPlayer]->isButtonPressed(A) && !backCheckButton[2 * enteringPlayer])
			{
				if (letterSel < 3)
					letterSel++;
				else
				{
					preNames[editSlot[enteringPlayer]].at(0) = L1;
					preNames[editSlot[enteringPlayer]].at(1) = L2;
					preNames[editSlot[enteringPlayer]].at(2) = L3;

					preText[editSlot[enteringPlayer]]->setMessage(std::to_string(editSlot[enteringPlayer] + 1) + ". " + preNames[editSlot[enteringPlayer]] + " ======= " + std::to_string((int)EPD::playerScores[enteringPlayer]));
					preText[editSlot[enteringPlayer]]->baseColor = vec3(1.f);
					for (unsigned int k = 0; k < preText[editSlot[enteringPlayer]]->messageSize(); k++)
					{
						preText[editSlot[enteringPlayer]]->colorShift[k] = playerScoreBars[enteringPlayer]->objectColor;
					}

					newEnter = true;
				}
			}
			if (controllers[enteringPlayer]->isButtonPressed(B) && !backCheckButton[2 * enteringPlayer + 1])
			{
				if (letterSel > 0)
					letterSel--;
			}

			backCheckTriggers[2 * enteringPlayer] = playerTriggers[enteringPlayer].LT;
			backCheckTriggers[2 * enteringPlayer + 1] = playerTriggers[enteringPlayer].RT;
			backCheckButton[2 * enteringPlayer] = controllers[enteringPlayer]->isButtonPressed(A);
			backCheckButton[2 * enteringPlayer + 1] = controllers[enteringPlayer]->isButtonPressed(B);
			backCheckSticks[2 * enteringPlayer] = vec2(playerInput[2 * enteringPlayer].x, playerInput[2 * enteringPlayer].y);
			backCheckSticks[2 * enteringPlayer + 1] = vec2(playerInput[2 * enteringPlayer + 1].x, playerInput[2 * enteringPlayer + 1].y);
		}
	}
	else if (!runThrough)
	{
		if (keysDown[' '] && !backCheckKeysDown[' '])
		{
			runThrough = true;
		}

		for (int i = 0; i < 4; i++)
		{
			if (controllers[i])
			{
				if (controllers[i]->isButtonPressed(A) && !backCheckButton[2 * i])
				{
					runThrough = true;
				}

				backCheckButton[2 * i] = controllers[i]->isButtonPressed(A);
				backCheckButton[2 * i + 1] = controllers[i]->isButtonPressed(B);
			}
		}
	}
	else
	{
		fadeOutTime += deltaTime * 0.6f;
		if (fadeOutTime > 1.f)
		{
			fadeOutTime = 1.f;
			_GS = GS_ENDED;
			tSwitch = rm::getTexture("SCENE_VICTORY_END");
			tFade = rm::getTexture("defaultWhite");
		}
	}

	if (newEnter)
	{
		enteringPlayer++;
		letterSel = 0;
	}

	protectedEntityShip(&lightShip, SUN);

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	performUpdates(deltaTime);
}

void VictoryScene::enterUpdate(float dt)
{
	victoryCam->update(deltaTime);
	sinWaveTime += deltaTime * 0.3f;
	if (sinWaveTime > 1.f)
	{
		sinWaveTime = 1.f;
		_GS = GS_RUNNING;
	}
	victoryCam->setLocalPos(vec3(22.5f - 22.5f * cos(sinWaveTime * 3.1415926535f), 0, 0));

	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	protectedEntityShip(&updateShip, (GameObject*)backdrop);

	drawChildren(&renderShip, &lightShip, backdrop, false);
	drawChildren(&renderShip, &lightShip, confirmation, false);
	if (leaderboarders > 0)
		drawChildren(&renderShip, &lightShip, nameInitials, false);
	for (int i = 0; i < 4; i++)
	{
		if (EPD::playerActive[i])
		{
			drawChildren(&renderShip, &lightShip, playerModels[i], false);
			drawChildren(&renderShip, &lightShip, playerScoreBars[i], false);
			drawChildren(&renderShip, &lightShip, playerScores[i], false);
		}
	}

	for (int i = 0; i < 20; i++)
	{
		protectedEntityShip(&updateShip, (GameObject*)preText[i]);
		drawChildren(&renderShip, &lightShip, preText[i], false);
	}

	protectedEntityShip(&lightShip, SUN);

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	performUpdates(deltaTime);
}

void VictoryScene::endedUpdate(float dt)
{
	tTime += 0.7f * deltaTime;
	if (tTime > 1.f)
	{
		tTime = 1.f;
		_GS = GS_EXITING;
	}
	victoryCam->update(deltaTime);

	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	protectedEntityShip(&updateShip, (GameObject*)backdrop);
	//protectedEntityShip(&updateShip, (GameObject*)U_A);
	//protectedEntityShip(&updateShip, (GameObject*)D_A);

	drawChildren(&renderShip, &lightShip, backdrop, false);
	drawChildren(&renderShip, &lightShip, confirmation, false);
	if (leaderboarders > 0)
		drawChildren(&renderShip, &lightShip, nameInitials, false);

	for (int i = 0; i < 20; i++)
	{
		//protectedEntityShip(&updateShip, (GameObject*)preText[i]);
		drawChildren(&renderShip, &lightShip, preText[i], false);
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	performUpdates(deltaTime);
}

void VictoryScene::exitingUpdate(float dt)
{
	std::ofstream HI_SC;
	HI_SC.open("../assets/High Scores/" + std::to_string(accum) + "_HS.txt");
	for (int i = 0; i < 20; i++)
	{
		if (preNames[i] != "---")
		{
			if (i > 0)
				HI_SC << "\n";
			HI_SC << preNames[i] << " " << std::to_string(preScores[i]);
		}
		
	}
	HI_SC.close();

	readyToTerminate = true;
	nextScene = "VEHICLE_SELECTION";
}

void VictoryScene::keyboardDown(unsigned char key, int mouseX, int mouseY)
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

void VictoryScene::keyboardUp(unsigned char key, int mouseX, int mouseY)
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

void VictoryScene::keyboardSpecialDown(int key, int mouseX, int mouseY)
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

void VictoryScene::keyboardSpecialUp(int key, int mouseX, int mouseY)
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

void VictoryScene::mouseClicked(int button, int state, int x, int y)
{
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
		ImGui::GetIO().MouseDown[0] = !state;
	}
	else
	{
	}

	if (state == GLUT_DOWN)
		mouseDown[button] = true;
	else
		mouseDown[button] = false;
}

void VictoryScene::mouseMoved(int x, int y)
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

void VictoryScene::mousePassive(int x, int y)
{
}

void VictoryScene::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	victoryCam->giveNewOrthoRatio(aspect);

	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);

	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(w, h);
	}
}

void VictoryScene::performUpdates(float dt)
{
	for (GameObject* object : updateShip)
	{
		object->update(dt);
	}
}

void VictoryScene::setUpVictory()
{
	for (int i = 0; i < 4; i++)
	{
		backCheckTriggers[2 * i] = 0.f;
		backCheckTriggers[2 * i + 1] = 0.f;
		backCheckButton[2 * i] = false;
		backCheckButton[2 * i + 1] = false;
		backCheckSticks[2 * i] = vec2(0, 0);
		backCheckSticks[2 * i + 1] = vec2(0, 0);
	}

	SUN = rm::getCloneOfLight("SUNNIER_LIGHT");
	SUN->setLocalRotY(30.f);
	SUN->update(0);

	victoryCam = rm::getCloneOfCamera("VICTORY_CAM");
	victoryCam->giveNewOrthoRatio(aspect);
	victoryCam->setLocalRotX(-30.f);
	victoryCam->update(0);

	backdrop = rm::getCloneOfRecolorObject("SCENE_VICTORY");
	backdrop->objectColor = vec3(0.3f, 0.3f, 0.3f);
	backdrop->setLocalRotX(90.f);
	backdrop->setLocalPos(vec3(0.f, 0.f, -50.f));
	backdrop->setScale(vec3(victoryCam->m_pOrthoSize.y, 1.f, victoryCam->m_pOrthoSize.w) / 3.f);
	victoryCam->addChild(backdrop);
	backdrop->update(0);

	nameInitials = rm::getCloneOfText("TextRot");
	nameInitials->setLocalPos(vec3(45.f, -4.8f, 0.f));
	nameInitials->setScale(0.4f);
	nameInitials->setMessage("P1: NUL");
	nameInitials->baseColor = vec3(1.f, 1.f, 1.f);

	for (unsigned int i = 0; i < nameInitials->messageSize(); i++)
	{
		nameInitials->colorShift[i] = vec3(1.f, 1.f, 1.f);
	}

	nameInitials->update(0);

	confirmation = rm::getCloneOfText("TextRot");
	confirmation->setLocalPos(vec3(50.f, -4.8f, 0.f));
	confirmation->setScale(0.4f);
	confirmation->setMessage("CONFIRM (A)");
	confirmation->baseColor = vec3(1.f);

	for (unsigned int i = 0; i < confirmation->messageSize(); i++)
	{
		confirmation->colorShift[i] = vec3(0.f, 1.f, 0.f);
	}

	confirmation->update(0);

	std::ifstream HI_SCORES;
	accum = 0;
	for (int i = 0; i < 4; i++)
	{
		editSlot[i] = -1;
		if (EPD::playerActive[i])// || i == 0)
			accum++;
	}

	//std::cout << accum << std::endl;
	HI_SCORES.open("../assets/High Scores/" + std::to_string(accum) + "_HS.txt");
	int iter = 0;
	std::string gap;
	while (std::getline(HI_SCORES, gap) && iter < 20)
	{
		preNames[iter] += gap.at(0);
		preNames[iter] += gap.at(1);
		preNames[iter] += gap.at(2);
		gap.erase(gap.begin() + 0, gap.begin() + 3);
		preScores[iter] = std::stoi(gap);
		preText[iter] = rm::getCloneOfText("TextRot");
		preText[iter]->setMessage(std::to_string(iter + 1) + ". " + preNames[iter] + " ======= " + std::to_string(preScores[iter]));
		preText[iter]->setLocalPos(vec3(45.f, 4.f - (-4.f + 0.6f * iter), 0.f));
		preText[iter]->setLocalRotX(-30.f);
		preText[iter]->setScale(0.25f);
		preText[iter]->baseColor = vec3(1.f, 1.f, 1.f);
		for (unsigned int j = 0; j < preText[iter]->messageSize(); j++)
		{
			preText[iter]->colorShift[j] = vec3(1.f);
		}

		iter++;
	}
	HI_SCORES.close();
	for (; iter < 20; iter++)
	{
		preNames[iter] = ("---");
		preScores[iter] = (0);
		preText[iter] = rm::getCloneOfText("TextRot");
		preText[iter]->setMessage(std::to_string(iter + 1) + ". " + preNames[iter] + " ======= " + std::to_string(preScores[iter]));
		preText[iter]->setLocalPos(vec3(45.f, 4.f - (-4.f + 0.6f * iter), 0.f));
		preText[iter]->setLocalRotX(-30.f);
		preText[iter]->setScale(0.25f);
		preText[iter]->baseColor = vec3(1.f, 1.f, 1.f);
		for (unsigned int j = 0; j < preText[iter]->messageSize(); j++)
		{
			preText[iter]->colorShift[j] = vec3(1.f);
		}
	}

	//for (int i = 0; i < 20; i++)
	//{
	//	std::cout << preNames[i] << " " << preScores[i] << std::endl;
	//}

	for (int i = 0; i < 4; i++)
	{
		playerModels[i] = nullptr;
		playerScoreBars[i] = nullptr;
		playerScores[i] = nullptr;
		playerScoresBACK[i] = nullptr;

		if (EPD::playerActive[i])
		{
			playerScoreBars[i] = rm::getCloneOfRecolorObject("CUBE");
			playerScores[i] = rm::getCloneOfText("TextRot");
			playerScoresBACK[i] = rm::getCloneOfText("TextRot");

			switch (EPD::playerVehicleChoice[i])
			{
			case 0:
				playerModels[i] = rm::getCloneOfPlayer("PLAYER_TRUCK");
				break;
			case 1:
				playerModels[i] = rm::getCloneOfPlayer("PLAYER_TANK");
				break;
			case 2:
				playerModels[i] = rm::getCloneOfPlayer("PLAYER_BULLDOZER");
				break;
			case 3:
				playerModels[i] = rm::getCloneOfPlayer("PLAYER_WRECKINGBALL");
				break;
			}
		}

		if (playerModels[i])
		{
			playerModels[i]->setLocalPos(vec3(-6.f + 4.f * i, -4.f, 0.f));
			playerModels[i]->setLocalRotY(135.f);
			playerModels[i]->update(0);

			scoreLerp[i] = 0.f;

			playerScoreBars[i]->setLocalPos(vec3(-6.f + 4.f * i, -4.f, 0.f));
			playerScoreBars[i]->setLocalRotY(45.f);
			playerScoreBars[i]->setScale(vec3(1.f, 0.01f, 1.f));

			switch (i)
			{
			case 0:
				playerScoreBars[i]->objectColor = vec3(0.4f, 0.4f, 1.f);
				break;
			case 1:
				playerScoreBars[i]->objectColor = vec3(1.f, 0.4f, 0.4f);
				break;
			case 2:
				playerScoreBars[i]->objectColor = vec3(0.4f, 1.f, 0.4f);
				break;
			case 3:
				playerScoreBars[i]->objectColor = vec3(1.f, 1.f, 0.4f);
				break;
			}

			playerScoreBars[i]->update(0);

			playerScores[i]->setMessage(std::to_string((int)scoreLerp[i]));
			playerScores[i]->setLocalPos(playerModels[i]->getWorldPos() + vec3(0.f, 3.f, 0.f));
			playerScores[i]->setLocalRotX(-30.f);
			playerScores[i]->setScale(0.6f);
			playerScores[i]->baseColor = vec3(1.f);
			playerScores[i]->update(0);

			playerScoresBACK[i]->setMessage(playerScores[i]->message);
			playerScores[i]->addChild(playerScoresBACK[i]);
			//playerScoresBACK[i]->setScale(1.05f);
			playerScoresBACK[i]->baseColor = vec3(1.f);
			playerScoresBACK[i]->update(0);

			for (unsigned int j = 0; j < playerScores[i]->messageSize(); j++)
			{
				playerScores[i]->colorShift[j] = playerScoreBars[i]->objectColor;
				playerScoresBACK[i]->colorShift[j] = vec3(1.f);
				playerScoresBACK[i]->tS[j] = vec3(1.1f);
			}
		}
	}

	U_A = rm::getCloneOfRecolorObject("ARROW_MODEL_LEFT");
	U_A->setLocalRotZ(-90.f);
	U_A->setScale(0.5f);
	U_A->update(0);

	D_A = rm::getCloneOfRecolorObject("ARROW_MODEL_RIGHT");
	D_A->setLocalRotZ(-90.f);
	D_A->setScale(0.5f);
	D_A->update(0);

	tSwitch = rm::getTexture("SCENE_VICTORY_BEGIN");
	tFade = rm::getTexture("defaultBlack");

	leaderboarders = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (EPD::playerActive[i] && EPD::playerScores[i] > preScores[j])
			{
				editSlot[i] = j;
				leaderboarders++;
				//std::cout << j << std::endl;
				Text* _T = rm::getCloneOfText("TextRot");
				_T->setMessage(std::to_string(j + 1) + ". ??? ======= " + std::to_string((int)EPD::playerScores[i]));
				_T->setLocalPos(vec3(45.f, 4.f - (-4.f + 0.6f * j), 0.f));
				_T->setLocalRotX(-30.f);
				_T->setScale(0.25f);
				_T->baseColor = vec3(1.f);
				for (unsigned int k = 0; k < _T->messageSize(); k++)
				{
					_T->colorShift[k] = vec3(1.f);
				}

				for (int k = 0; k < 4; k++)
				{
					if (k != i && editSlot[k] >= j)
					{
						editSlot[k]++;
						if (editSlot[k] >= 20)
						{
							editSlot[k] = -1;
							leaderboarders--;
						}
					}
				}

				preText.insert(preText.begin() + j, _T);
				preText.erase(preText.end() - 1);
				preNames.insert(preNames.begin() + j, "???");
				preNames.erase(preNames.end() - 1);
				preScores.insert(preScores.begin() + j, (int)EPD::playerScores[i]);
				preScores.erase(preScores.end() - 1);

				for (int I = j + 1; I < 20; I++)
				{
					preText[I]->setLocalPos(vec3(45.f, 4.f - (-4.f + 0.6f * I), 0.f));
					preText[I]->setMessage(std::to_string(I + 1) + ". " + preNames[I] + " ======= " + std::to_string(preScores[I]));
				}
				j = 20;
			}
		}
	}

	if (leaderboarders == 0)
	{
		confirmation->setMessage("CONTINUE (A)");
		confirmation->baseColor = vec3(1.f);

		for (unsigned int i = 0; i < confirmation->messageSize(); i++)
		{
			confirmation->colorShift[i] = vec3(0.f, 1.f, 0.f);
		}
	}

	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(windowWidth, windowHeight);
	}
}
