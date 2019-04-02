#include "Title.h"

Title::Title()
{
	updateTimer = new Timer();
}

Title::~Title()
{
	delete updateTimer;
}

void Title::initializeGame()
{
	readyToTerminate = false;
	TotalTime = 0.f;
	tTime = 1.0001f;
	TotalGameTime = 0.f;
	flyOut = 0.f;

	menuSelection = 0;

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	backCheckTriggers.clear();
	backCheckButton.clear();
	backCheckSticks.clear();

	backCheckTriggers.resize(8);
	backCheckButton.resize(8);
	backCheckSticks.resize(8);

	setUpTitle();

	titleCam->update(0);
	titleCam->sendUBO();

	updateTimer->tick();
	deltaTime = updateTimer->getElapsedTimeSeconds();

	for (unsigned int i = 0; i < 256; i++)
	{
		setKeysDown(false, (unsigned char)i);
	}

	for (int i = 0; i < 4; i++)
	{
		Sound::engine.listener[i].listening = false;
	}
	Sound::engine.listener[0].listening = true;

	_GS = GS_BEGINNING;

	backgroundMusic->Stop(backgroundChannel);
	backgroundMusic = rm::getSound("TITLE_MUSIC");
	backgroundChannel = backgroundMusic->Play();
	backgroundChannel->setVolume(1.f);
}

void Title::update()
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
	TotalTime += deltaTime;

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

void Title::draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();

	titleCam->sendUBO();
	titleCam->setRenderList(renderShip);
	titleCam->cull();
	titleCam->render();

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

void Title::GUI()
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

	vec3 cPos = titleCam->getLocalPos();
	ImGui::SliderFloat3("CAMERA_POSITION", &cPos.x, -10, 10, "%.5f");
	titleCam->setLocalPos(cPos);

	ImGui::End();

	UI::End();
}

void Title::releaseScene()
{
	rm::order66();
}

void Title::beginningUpdate(float dt)
{
	_GS = GS_STARTING;
}

void Title::startingUpdate(float dt)
{
	tTime -= deltaTime * 0.2f;
	//std::cout << tTime << std::endl;
	if (tTime < 0.f)
	{
		_GS = GS_RUNNING;
		tTime = 0.f;
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	//titleCam->setLocalRotY(TotalGameTime * 90.f);
	//titleCam->setLocalRotY(180.f);
	titleCam->update(deltaTime);

	ARROW->setLocalRotZ(sin(TotalGameTime * 3.f) * 10.f);
	ARROW->setScale(1.f);

	START->setScale(1.8f);
	RULES->setScale(1.8f);
	EXIT->setScale(1.8f);

	for (unsigned int i = 0; i < DOWN_TOWN->messageSize(); i++)
	{
		DOWN_TOWN->colorShift[i] = vec3(1.f, 0.5f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DOWN_TOWN->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i), 12);
	}
	for (unsigned int i = 0; i < DESTRUCTION->messageSize(); i++)
	{
		DESTRUCTION->colorShift[i] = vec3(1.f, 0.f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DESTRUCTION->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i + 0.9f), 12);
	}

	drawChildren(&renderShip, &lightShip, backdrop, true);
	drawChildren(&renderShip, &lightShip, ARROW, true);
	drawChildren(&renderShip, &lightShip, START, true);
	drawChildren(&renderShip, &lightShip, RULES, true);
	drawChildren(&renderShip, &lightShip, EXIT, true);
	drawChildren(&renderShip, &lightShip, building1, true);
	drawChildren(&renderShip, &lightShip, building2, true);
	drawChildren(&renderShip, &lightShip, tree, true);
	drawChildren(&renderShip, &lightShip, DOWN_TOWN, true);
	drawChildren(&renderShip, &lightShip, DESTRUCTION, true);
	drawChildren(&renderShip, &lightShip, playerModel, true);

	protectedEntityShip(&updateShip, backdrop);
	protectedEntityShip(&updateShip, (GameObject*)ARROW);
	protectedEntityShip(&updateShip, (GameObject*)START);
	protectedEntityShip(&updateShip, (GameObject*)RULES);
	protectedEntityShip(&updateShip, (GameObject*)EXIT);
	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	//protectedEntityShip(&updateShip, (GameObject*)DOWN_TOWN);
	//protectedEntityShip(&updateShip, (GameObject*)DESTRUCTION);

	performUpdates(deltaTime);
}

void Title::runningUpdate(float dt)
{
	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	//titleCam->setLocalRotY(TotalGameTime * 90.f);
	//titleCam->setLocalRotY(180.f);
	titleCam->update(deltaTime);

	ARROW->setLocalRotZ(sin(TotalGameTime * 3.f) * 10.f);

	START->setScale(1.8f);
	RULES->setScale(1.8f);
	EXIT->setScale(1.8f);

	for (unsigned int i = 0; i < DOWN_TOWN->messageSize(); i++)
	{
		DOWN_TOWN->colorShift[i] = vec3(1.f, 0.5f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DOWN_TOWN->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i), 12);
	}
	for (unsigned int i = 0; i < DESTRUCTION->messageSize(); i++)
	{
		DESTRUCTION->colorShift[i] = vec3(1.f, 0.f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DESTRUCTION->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i + 0.9f), 12);
	}

	if (keysDown['w'] && !backCheckKeysDown['w'])
	{
		if (menuSelection > 0)
			menuSelection--;
		ARROW->setScale(2.f);
	}
	if (keysDown['s'] && !backCheckKeysDown['s'])
	{
		if (menuSelection < 2)
			menuSelection++;
		ARROW->setScale(2.f);
	}

	if (keysDown[' '] && !backCheckKeysDown[' '])
	{
		_GS = GS_ENDED;
		tSwitch = rm::getTexture("SCENE_MENU_END");
		tFade = rm::getTexture("defaultBlack");
		curPos = ARROW->getLocalPos();
		curRotZ = ARROW->getLocalEuler().z;
		repScene = menuSelection;
	}

	for (int i = 0; i < 4; i++)
	{
		if (controllers[i])
		{
			controllers[i]->getSticks(&playerInput[2 * i], &playerInput[2 * i + 1]);
			controllers[i]->getTriggers(&playerTriggers[i]);

			if (playerInput[2 * i].y > 0.5f && !(backCheckSticks[2 * i].y > 0.5f))
			{
				if (menuSelection > 0)
					menuSelection--;
				ARROW->setScale(2.f);
			}
			if (playerInput[2 * i].y < -0.5f && !(backCheckSticks[2 * i].y < -0.5f))
			{
				if (menuSelection < 2)
					menuSelection++;
				ARROW->setScale(2.f);
			}

			if (controllers[i]->isButtonPressed(A) && !backCheckButton[2 * i])
			{
				_GS = GS_ENDED;
				tSwitch = rm::getTexture("SCENE_MENU_END");
				tFade = rm::getTexture("defaultBlack");
				curPos = ARROW->getLocalPos();
				curRotZ = ARROW->getLocalEuler().z;
				repScene = menuSelection;
			}

			backCheckTriggers[2 * i] = playerTriggers[i].LT;
			backCheckTriggers[2 * i + 1] = playerTriggers[i].RT;
			backCheckButton[2 * i] = controllers[i]->isButtonPressed(A);
			backCheckButton[2 * i + 1] = controllers[i]->isButtonPressed(B);
			backCheckSticks[2 * i] = vec2(playerInput[2 * i].x, playerInput[2 * i].y);
			backCheckSticks[2 * i + 1] = vec2(playerInput[2 * i + 1].x, playerInput[2 * i + 1].y);
		}
	}

	ARROW->setScale(ARROW->getLocalScale() * pow(0.92f, 60.f * deltaTime));
	if (ARROW->getLocalScale().x < 1.f || ARROW->getLocalScale().y < 1.f || ARROW->getLocalScale().z < 1.f)
	{
		ARROW->setScale(1.f);
	}

	switch (menuSelection)
	{
	case 0:
		START->setScale(2.5f);
		ARROW->setLocalPos(START->getLocalPos() + vec3(2.f, 0, 0));
		ARROW->objectColor = START->objectColor;
		break;
	case 1:
		RULES->setScale(2.5f);
		ARROW->setLocalPos(RULES->getLocalPos() + vec3(2.f, 0, 0));
		ARROW->objectColor = RULES->objectColor;
		break;
	case 2:
		EXIT->setScale(2.5f);
		ARROW->setLocalPos(EXIT->getLocalPos() + vec3(2.f, 0, 0));
		ARROW->objectColor = EXIT->objectColor;
		break;
	}



	drawChildren(&renderShip, &lightShip, backdrop, true);
	drawChildren(&renderShip, &lightShip, ARROW, true);
	drawChildren(&renderShip, &lightShip, START, true);
	drawChildren(&renderShip, &lightShip, RULES, true);
	drawChildren(&renderShip, &lightShip, EXIT, true);
	drawChildren(&renderShip, &lightShip, building1, true);
	drawChildren(&renderShip, &lightShip, building2, true);
	drawChildren(&renderShip, &lightShip, tree, true);
	drawChildren(&renderShip, &lightShip, DOWN_TOWN, true);
	drawChildren(&renderShip, &lightShip, DESTRUCTION, true);
	drawChildren(&renderShip, &lightShip, playerModel, true);

	protectedEntityShip(&updateShip, backdrop);
	protectedEntityShip(&updateShip, (GameObject*)ARROW);
	protectedEntityShip(&updateShip, (GameObject*)START);
	protectedEntityShip(&updateShip, (GameObject*)RULES);
	protectedEntityShip(&updateShip, (GameObject*)EXIT);
	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	//protectedEntityShip(&updateShip, (GameObject*)DOWN_TOWN);
	//protectedEntityShip(&updateShip, (GameObject*)DESTRUCTION);

	performUpdates(deltaTime);
}

void Title::endedUpdate(float dt)
{
	//_GS = GS_EXITING;
	flyOut += deltaTime;

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	//titleCam->setLocalRotY(TotalGameTime * 90.f);
	//titleCam->setLocalRotY(180.f);
	titleCam->update(deltaTime);

	if (flyOut < keyPos1)
	{
		float tV = 1.f - (keyPos1 - flyOut) / keyPos1;
		ARROW->setLocalRotZ(lerp(curRotZ, 1800.f, 1.f - pow(1.f - tV, 2)));
		ARROW->setLocalPos(lerp(curPos, vec3(3.0f, 2.0f, 3), 1.f - pow(1.f - tV, 2)));
	}
	else
	{
		_GS = GS_EXITING;
	}

	//ARROW->setLocalRotZ(sin(TotalGameTime * 3.f) * 10.f);
	//ARROW->setScale(1.f);


	

	//START->setScale(1.8f);
	//RULES->setScale(1.8f);
	//EXIT->setScale(1.8f);

	for (unsigned int i = 0; i < DOWN_TOWN->messageSize(); i++)
	{
		DOWN_TOWN->colorShift[i] = vec3(1.f, 0.5f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DOWN_TOWN->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i), 12);
	}
	for (unsigned int i = 0; i < DESTRUCTION->messageSize(); i++)
	{
		DESTRUCTION->colorShift[i] = vec3(1.f, 0.f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DESTRUCTION->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i + 0.9f), 12);
	}

	drawChildren(&renderShip, &lightShip, backdrop, true);
	drawChildren(&renderShip, &lightShip, ARROW, true);
	drawChildren(&renderShip, &lightShip, START, true);
	drawChildren(&renderShip, &lightShip, RULES, true);
	drawChildren(&renderShip, &lightShip, EXIT, true);
	drawChildren(&renderShip, &lightShip, building1, true);
	drawChildren(&renderShip, &lightShip, building2, true);
	drawChildren(&renderShip, &lightShip, tree, true);
	drawChildren(&renderShip, &lightShip, DOWN_TOWN, true);
	drawChildren(&renderShip, &lightShip, DESTRUCTION, true);
	drawChildren(&renderShip, &lightShip, playerModel, true);

	protectedEntityShip(&updateShip, backdrop);
	protectedEntityShip(&updateShip, (GameObject*)ARROW);
	protectedEntityShip(&updateShip, (GameObject*)START);
	protectedEntityShip(&updateShip, (GameObject*)RULES);
	protectedEntityShip(&updateShip, (GameObject*)EXIT);
	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	//protectedEntityShip(&updateShip, (GameObject*)DOWN_TOWN);
	//protectedEntityShip(&updateShip, (GameObject*)DESTRUCTION);

	performUpdates(deltaTime);
}

void Title::exitingUpdate(float dt)
{
	flyOut += deltaTime;

	if (tTime < 1.0001f)
	{
		tTime += deltaTime * 1.5f;
	}
	else
	{
		readyToTerminate = true;
		if (menuSelection == 0)
			nextScene = "VEHICLE_SELECTION";
		else if (menuSelection == 1)
			nextScene = "RULES";
		else
			exit(1);
		//for (unsigned int i = 0; i < rm::TransformsINGAME.size(); i++)
		//{
		//	std::cout << rm::TransformsINGAME[i]->getName() << std::endl;
		//}
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	//titleCam->setLocalRotY(TotalGameTime * 90.f);
	//titleCam->setLocalRotY(180.f);
	titleCam->update(deltaTime);

	if (flyOut < keyPos2)
	{
		float tV = 1.f - (flyOut - keyPos2) / (keyPos1 - keyPos2);
		ARROW->setLocalRotZ(0);
		ARROW->setLocalPos(lerp(vec3(3.0f, 2.0f, 3), vec3(-10.f, 2.5f, 3), tV));
		ARROW->setScale(lerp(vec3(1), vec3(2.f, 1.f, 1.f), 1.f - pow(1.f - tV, 2)));
	}

	//ARROW->setLocalRotZ(sin(TotalGameTime * 3.f) * 10.f);
	//ARROW->setScale(1.f);




	//START->setScale(1.8f);
	//RULES->setScale(1.8f);
	//EXIT->setScale(1.8f);

	for (unsigned int i = 0; i < DOWN_TOWN->messageSize(); i++)
	{
		DOWN_TOWN->colorShift[i] = vec3(1.f, 0.5f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DOWN_TOWN->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i), 12);
	}
	for (unsigned int i = 0; i < DESTRUCTION->messageSize(); i++)
	{
		DESTRUCTION->colorShift[i] = vec3(1.f, 0.f, 0.f) * (1.f - pow(sin(TotalGameTime + 0.1f * i), 4));
		DESTRUCTION->posOffset[i] = vec3(0.f, 0.4f, 0.f) * pow(sin(TotalGameTime * 2.31f + 0.1f * i + 0.9f), 12);
	}

	drawChildren(&renderShip, &lightShip, backdrop, true);
	drawChildren(&renderShip, &lightShip, ARROW, true);
	drawChildren(&renderShip, &lightShip, START, true);
	drawChildren(&renderShip, &lightShip, RULES, true);
	drawChildren(&renderShip, &lightShip, EXIT, true);
	drawChildren(&renderShip, &lightShip, building1, true);
	drawChildren(&renderShip, &lightShip, building2, true);
	drawChildren(&renderShip, &lightShip, tree, true);
	drawChildren(&renderShip, &lightShip, DOWN_TOWN, true);
	drawChildren(&renderShip, &lightShip, DESTRUCTION, true);
	drawChildren(&renderShip, &lightShip, playerModel, true);

	protectedEntityShip(&updateShip, backdrop);
	protectedEntityShip(&updateShip, (GameObject*)ARROW);
	protectedEntityShip(&updateShip, (GameObject*)START);
	protectedEntityShip(&updateShip, (GameObject*)RULES);
	protectedEntityShip(&updateShip, (GameObject*)EXIT);
	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	//protectedEntityShip(&updateShip, (GameObject*)DOWN_TOWN);
	//protectedEntityShip(&updateShip, (GameObject*)DESTRUCTION);

	performUpdates(deltaTime);
}

void Title::keyboardDown(unsigned char key, int mouseX, int mouseY)
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

void Title::keyboardUp(unsigned char key, int mouseX, int mouseY)
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

void Title::keyboardSpecialDown(int key, int mouseX, int mouseY)
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

void Title::keyboardSpecialUp(int key, int mouseX, int mouseY)
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

void Title::mouseClicked(int button, int state, int x, int y)
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

void Title::mouseMoved(int x, int y)
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

void Title::mousePassive(int x, int y)
{
}

void Title::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	titleCam->perspective(60.f, aspect, 1, 1000);

	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);
	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(w, h);
	}
}

void Title::performUpdates(float dt)
{
	for (GameObject* object : updateShip)
	{
		object->update(dt);
	}
}

void Title::setUpTitle()
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

	titleCam = rm::getCloneOfCamera("TITLE_CAM");

	START = rm::getCloneOfRecolorObject("BUTTON_START");
	RULES = rm::getCloneOfRecolorObject("BUTTON_RULES");
	EXIT = rm::getCloneOfRecolorObject("BUTTON_EXIT");
	ARROW = rm::getCloneOfRecolorObject("ARROW_MODEL_LEFT");

	backdrop = rm::getCloneOfObject("SCENE_MENU");

	building1 = rm::getCloneOfDestructable("BUILDING 1");
	building2 = rm::getCloneOfDestructable("BUILDING 2");
	tree = rm::getCloneOfDestructable("PINE TREE 1");

	playerModel = rm::getCloneOfPlayer("PLAYER_TRUCK");
	playerModel->setLocalPos(vec3(-3.7f, 0.375f, 0));
	playerModel->setScale(0.6f);
	playerModel->setLocalRotY(-170.f);
	playerModel->update(0);

	SUN = rm::getCloneOfLight("SUNNIER_LIGHT");
	SUN2 = rm::getCloneOfLight("SUNLIGHT");
	SUN->update(0);
	SUN2->setLocalRotZ(180.f);
	SUN2->setLocalRotY(90.f);
	SUN2->update(0);
	//std::cout << SUN->getLocalToWorld() * SUN->direction << std::endl;
	//std::cout << SUN2->getLocalToWorld() * SUN2->direction << std::endl;

	DOWN_TOWN = rm::getCloneOfText("TextRot");
	DOWN_TOWN->setMessage("DOWN TOWN");
	DOWN_TOWN->setLocalPos(vec3(2.f, 2.8f, -1.f));
	DOWN_TOWN->setScale(0.4f);
	DOWN_TOWN->setLocalRotY(-30.f);
	DOWN_TOWN->baseColor = vec3(1.f, 1.f, 1.f);
	DOWN_TOWN->update(0);


	DESTRUCTION = rm::getCloneOfText("TextRot");
	DESTRUCTION->setMessage("DESTRUCTION");
	DESTRUCTION->setLocalPos(vec3(2.f, 2.f, -1.f));
	DESTRUCTION->setScale(0.4f);
	DESTRUCTION->setLocalRotY(-30.f);
	DESTRUCTION->baseColor = vec3(1.f, 1.f, 1.f);
	DESTRUCTION->update(0);


	backdrop->setLocalPos(vec3(0, 0, 0));
	backdrop->update(0);

	titleCam->setLocalPos(vec3(0.25, 2, 5));
	titleCam->giveNewPersRatio(aspect);
	titleCam->PLAYER = &Sound::engine.listener[0];
	titleCam->PLAYER->pos = Transform::toFV(titleCam->getLocalPos());
	titleCam->update(0);

	START->setLocalPos(vec3(-2.2f, 3.5f, 0));
	START->setScale(1.8f);
	START->setLocalRotY(30.f);
	START->objectColor = vec3(1.f, 0.f, 0.f);
	START->update(0);

	RULES->setLocalPos(vec3(-2.2f, 2.5f, 0));
	RULES->setScale(1.8f);
	RULES->setLocalRotY(30.f);
	RULES->objectColor = vec3(1.f, 0.5f, 0.f);
	RULES->update(0);

	EXIT->setLocalPos(vec3(-2.2f, 1.5f, 0));
	EXIT->setScale(1.8f);
	EXIT->setLocalRotY(30.f);
	EXIT->objectColor = vec3(1.f, 1.f, 0.f);
	EXIT->update(0);

	ARROW->setLocalPos(START->getLocalPos() + vec3(2.f, 0, 0));
	ARROW->setScale(1.f);
	ARROW->setLocalRotZ(0.f);
	ARROW->objectColor = START->objectColor;
	ARROW->update(0);

	//Sound* SUND = rm::getSound("drumloop");
	//ARROW->sound = SUND->Play(Transform::toFV(ARROW->getLocalPos()));

	building1->setLocalPos(vec3(3.2f, 0, -8.f));
	building1->setLocalRotY(30.f);
	building1->setScale(1.2f);
	building1->setBob(((float)(rand() % 629)) * 0.01f + 6.28f);
	building1->update(0);

	building2->setLocalPos(vec3(-1.2f, 0, -6.f));
	building2->setLocalRotY(-45.f);
	building2->setScale(1.2f);
	building2->setBob(((float)(rand() % 629)) * 0.01f + 6.28f);
	building2->update(0);

	tree->setLocalPos(vec3(0.7f, 0.f, -4.6f));
	tree->setLocalRotY(60.f);
	tree->setBob(((float)(rand() % 629)) * 0.01f + 6.28f);
	tree->update(0);

	tSwitch = rm::getTexture("SCENE_BEGIN_MENU");
	tFade = rm::getTexture("FADE_BEGIN_MENU");
	//START->setLocalPos(vec3());
}
