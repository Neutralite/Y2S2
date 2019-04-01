#include "VehicleSelection.h"
#include "ExternalPlayerData.h"

VehicleSelection::VehicleSelection()
{
	updateTimer = new Timer();
}

VehicleSelection::~VehicleSelection()
{
	delete updateTimer;
}

void VehicleSelection::initializeGame()
{
	EPD::initEPD();

	readyToTerminate = false;
	TotalTime = 0.f;
	tTime = 1.0001f;
	TotalGameTime = 0.f;
	backTimer = 0.f;
	repScene = 0;

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	playerStillModel.clear();
	playerView.clear();
	playerSelect.clear();
	playerReady.clear();
	sceneButtons.clear();
	READY.clear();
	READY_TIME.clear();
	backCheckTriggers.clear();
	backCheckButton.clear();
	backCheckSticks.clear();

	playerStillModel.resize(4);
	playerView.resize(4);
	playerSelect.resize(4);
	playerReady.resize(4);
	sceneButtons.resize(8);
	READY.resize(4);
	READY_TIME.resize(4);
	backCheckTriggers.resize(8);
	backCheckButton.resize(8);
	backCheckSticks.resize(8);

	setUpVehicleSelect();

	vehicleSelectCam->update(0);
	vehicleSelectCam->sendUBO();

	updateTimer->tick();
	deltaTime = updateTimer->getElapsedTimeSeconds();

	for (unsigned int i = 0; i < 256; i++)
	{
		setKeysDown(false, (unsigned char)i);
	}

	_GS = GS_BEGINNING;
}

void VehicleSelection::update()
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

	//uniformBufferTime.sendFloat(TotalGameTime, 0);

	//performUpdates(deltaTime);
}

void VehicleSelection::draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();

	vehicleSelectCam->sendUBO();
	vehicleSelectCam->setRenderList(renderShip);
	vehicleSelectCam->cull();
	vehicleSelectCam->render();

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

void VehicleSelection::GUI()
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

	vec3 cPos = vehicleSelectCam->getLocalPos();
	ImGui::SliderFloat3("CAMERA_POSITION", &cPos.x, -10, 10, "%.5f");
	vehicleSelectCam->setLocalPos(cPos);

	ImGui::End();

	UI::End();
}

void VehicleSelection::releaseScene()
{
	rm::order66();
}

void VehicleSelection::beginningUpdate(float dt)
{
	_GS = GS_STARTING;
}

void VehicleSelection::startingUpdate(float dt)
{
	tTime -= deltaTime * 1.2f;
	//std::cout << tTime << std::endl;
	if (tTime < 0.f)
	{
		_GS = GS_RUNNING;
		tTime = 0.f;
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	vehicleSelectCam->update(deltaTime);

	float mAm = backTimer * TEXT_BACK->messageSize();

	for (unsigned int i = 0; i < TEXT_BACK->messageSize(); i++)
	{
		if (mAm >= i + 1)
			TEXT_BACK->colorShift[i] = vec3(1.f, 0.1f, 0.1f);
		else
			TEXT_BACK->colorShift[i] = vec3(1.f, 1.f, 1.f);
	}

	for (int i = 0; i < 4; i++)
	{
		playerView[i]->setLocalRotY(playerView[i]->getLocalEuler().y + 30.f * deltaTime);
		playerView[i]->removeChild((unsigned int)0);
		if (controllers[i])
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		else if (i == 0)
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		protectedEntityShip(&updateShip, playerView[i]);

		drawChildren(&renderShip, &lightShip, playerView[i], false);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i], true);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i + 1], true);
		drawChildren(&renderShip, &lightShip, READY[i], true);
	}
	drawChildren(&renderShip, &lightShip, backDrop, true);
	drawChildren(&renderShip, &lightShip, TEXT_BACK, true);
	//drawChildren(&renderShip, &lightShip, backButton, true);
	
	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	protectedEntityShip(&updateShip, (GameObject*)TEXT_BACK);

	performUpdates(deltaTime);
}

void VehicleSelection::runningUpdate(float dt)
{
	bool checkToSwap = true;
	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	vehicleSelectCam->update(deltaTime);

	float mAm = backTimer * TEXT_BACK->messageSize();

	for (unsigned int i = 0; i < TEXT_BACK->messageSize(); i++)
	{
		if (mAm >= i + 1)
		{
			TEXT_BACK->colorShift[i] = vec3(1.f, 0.1f, 0.1f);
			TEXT_BACK->tS[i] = vec3(1.3f);
		}
		else
		{
			TEXT_BACK->colorShift[i] = vec3(1.f, 1.f, 1.f);
			TEXT_BACK->tS[i] = vec3(1.f);
		}
	}

	if (keysDown['a'] && !backCheckKeysDown['a'] && !playerReady[0])
	{
		playerSelect[0] = (playerSelect[0] - 1) % 4;
		while (playerSelect[0] < 0)
			playerSelect[0] += 4;
		sceneButtons[0]->setScale(1.6f);
	}
	if (keysDown['d'] && !backCheckKeysDown['d'] && !playerReady[0])
	{
		playerSelect[0] = (playerSelect[0] + 1) % 4;
		while (playerSelect[0] > 3)
			playerSelect[0] -= 4;
		sceneButtons[1]->setScale(1.6f);
	}
	if (keysDown[' '] && !backCheckKeysDown[' '] && !playerReady[0])
	{
		playerReady[0] = true;
		READY[0]->setScale(0.35f);
	}
	if (keysDown['q'] && !backCheckKeysDown['q'])
	{
		playerReady[0] = false;
		READY[0]->setScale(0.2f);
	}

	bool goingBack = false;
	if (keysDown['b'])
	{
		goingBack = true;
	}

	bool controllerConnected = false;
	controllerConnected = true;
	if (READY_TIME[0] < 1.f)
	{
		checkToSwap = false;
	}

	for (int i = 0; i < 4; i++)
	{
		if (controllers[i])
		{
			controllerConnected = true;
			if (READY_TIME[i] < 1.f)
				checkToSwap = false;
			controllers[i]->getSticks(&playerInput[2 * i], &playerInput[2 * i + 1]);
			controllers[i]->getTriggers(&playerTriggers[i]);

			if (((playerTriggers[i].LT > 0.5f && !(backCheckTriggers[2 * i] > 0.5f))
				|| (playerInput[2 * i].x < -0.2f && !(backCheckSticks[2 * i].x < -0.2f))) && !playerReady[i])
			{
				playerSelect[i] = (playerSelect[i] - 1) % 4;
				while (playerSelect[i] < 0)
					playerSelect[i] += 4;
				sceneButtons[2 * i]->setScale(1.6f);
			}
			if (((playerTriggers[i].RT > 0.5f && !(backCheckTriggers[2 * i + 1] > 0.5f))
				|| (playerInput[2 * i].x > 0.2f && !(backCheckSticks[2 * i].x > 0.2f))) && !playerReady[i])
			{
				playerSelect[i] = (playerSelect[i] + 1) % 4;
				while (playerSelect[i] > 3)
					playerSelect[i] -= 4;
				sceneButtons[2 * i + 1]->setScale(1.6f);
			}
			if (controllers[i]->isButtonPressed(A) && !backCheckButton[2 * i] && !playerReady[i])
			{
				playerReady[i] = true;
				READY[i]->setScale(0.35f);
			}
			if (controllers[i]->isButtonPressed(B) && !backCheckButton[2 * i + 1])
			{
				playerReady[i] = false;
				READY[i]->setScale(0.2f);
			}
			if (controllers[i]->isButtonPressed(B))
			{
				goingBack = true;
			}

			backCheckTriggers[2 * i] = playerTriggers[i].LT;
			backCheckTriggers[2 * i + 1] = playerTriggers[i].RT;
			backCheckButton[2 * i] = controllers[i]->isButtonPressed(A);
			backCheckButton[2 * i + 1] = controllers[i]->isButtonPressed(B);
			backCheckSticks[2 * i] = playerInput[2 * i];
			backCheckSticks[2 * i + 1] = playerInput[2 * i + 1];
		}

		if (playerReady[i])
		{
			READY[i]->HIDE = false;
			READY_TIME[i] += deltaTime;
			if (READY_TIME[i] > 1.f)
				READY_TIME[i] = 1.f;
			READY[i]->setScale(max(READY[i]->getLocalScale() * pow(0.9f, 60.f * deltaTime), 0.2f));
		}
		else
		{
			READY[i]->HIDE = true;
			READY_TIME[i] = 0.f;
		}

		playerView[i]->setLocalRotY(playerView[i]->getLocalEuler().y + 30.f * deltaTime);
		playerView[i]->removeChild((unsigned int)0);
		if (controllers[i])
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		else if (i == 0)
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		protectedEntityShip(&updateShip, playerView[i]);

		sceneButtons[2 * i]->setScale(max(sceneButtons[2 * i]->getLocalScale() * pow(0.88f, 60.f * deltaTime), 0.7f));
		sceneButtons[2 * i + 1]->setScale(max(sceneButtons[2 * i + 1]->getLocalScale() * pow(0.88f, 60.f * deltaTime), 0.7f));

		protectedEntityShip(&updateShip, (GameObject*)sceneButtons[2 * i]);
		protectedEntityShip(&updateShip, (GameObject*)sceneButtons[2 * i + 1]);
		protectedEntityShip(&updateShip, (GameObject*)READY[i]);

		drawChildren(&renderShip, &lightShip, playerView[i], false);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i], true);
		drawChildren(&renderShip, &lightShip, TEXT_BACK, true);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i + 1], true);
		drawChildren(&renderShip, &lightShip, READY[i], true);
	}

	drawChildren(&renderShip, &lightShip, backDrop, true);
	drawChildren(&renderShip, &lightShip, TEXT_BACK, true);
	//drawChildren(&renderShip, &lightShip, backButton, true);

	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	protectedEntityShip(&updateShip, (GameObject*)TEXT_BACK);

	performUpdates(deltaTime);

	if (controllerConnected && checkToSwap)
	{
		_GS = GS_ENDED;
		int playerIter = 0;
		for (int i = 0; i < 4; i++)
		{
			if (controllers[i])
			{
				EPD::playerNumbers[i] = playerIter;
				EPD::playerActive[i] = true;
				EPD::playerVehicleChoice[i] = playerSelect[i];
				playerIter++;
			}
			//else if (i == 0)
			//{
			//	EPD::playerNumbers[i] = playerIter;
			//	EPD::playerActive[i] = true;
			//	EPD::playerVehicleChoice[i] = playerSelect[i];
			//	playerIter++;
			//}
		}

		tSwitch = rm::getTexture("SCENE_VEHICLE_EXIT");
		tFade = rm::getTexture("defaultBlack");
	}
	if (goingBack)
	{
		backTimer += deltaTime * 0.5f;
		if (backTimer > 1.f)
		{
			//backTimer = 0.f;

			_GS = GS_ENDED;
			repScene = 1;

			tSwitch = rm::getTexture("SCENE_VEHICLE_EXIT");
			tFade = rm::getTexture("defaultBlack");
		}
	}
	else
	{
		backTimer = 0.f;
	}
}

void VehicleSelection::endedUpdate(float dt)
{
	tTime += deltaTime * 1.2f;
	//std::cout << tTime << std::endl;
	if (tTime > 1.f)
	{
		_GS = GS_EXITING;
		tTime = 1.f;
	}

	TotalGameTime += deltaTime;

	uniformBufferTime.sendFloat(TotalGameTime, 0);

	vehicleSelectCam->update(deltaTime);

	float mAm = backTimer * TEXT_BACK->messageSize();

	for (unsigned int i = 0; i < TEXT_BACK->messageSize(); i++)
	{
		if (mAm >= i + 1)
			TEXT_BACK->colorShift[i] = vec3(1.f, 0.1f, 0.1f);
		else
			TEXT_BACK->colorShift[i] = vec3(1.f, 1.f, 1.f);
	}

	for (int i = 0; i < 4; i++)
	{
		playerView[i]->setLocalRotY(playerView[i]->getLocalEuler().y + 30.f * deltaTime);
		playerView[i]->removeChild((unsigned int)0);
		if (controllers[i])
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		else if (i == 0)
			playerView[i]->addChild(playerStillModel[playerSelect[i]][i]);
		protectedEntityShip(&updateShip, playerView[i]);

		drawChildren(&renderShip, &lightShip, playerView[i], false);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i], true);
		drawChildren(&renderShip, &lightShip, sceneButtons[2 * i + 1], true);
		drawChildren(&renderShip, &lightShip, READY[i], true);
	}
	drawChildren(&renderShip, &lightShip, backDrop, true);
	drawChildren(&renderShip, &lightShip, TEXT_BACK, true);
	//drawChildren(&renderShip, &lightShip, backButton, true);

	protectedEntityShip(&lightShip, SUN);
	protectedEntityShip(&lightShip, SUN2);
	protectedEntityShip(&updateShip, (GameObject*)TEXT_BACK);

	performUpdates(deltaTime);
}

void VehicleSelection::exitingUpdate(float dt)
{
	readyToTerminate = true;
	if (repScene == 0)
		nextScene = "MAIN_LOOP";
	else
		nextScene = "TITLE_SCREEN";
}

void VehicleSelection::keyboardDown(unsigned char key, int mouseX, int mouseY)
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

void VehicleSelection::keyboardUp(unsigned char key, int mouseX, int mouseY)
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

void VehicleSelection::keyboardSpecialDown(int key, int mouseX, int mouseY)
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

void VehicleSelection::keyboardSpecialUp(int key, int mouseX, int mouseY)
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

void VehicleSelection::mouseClicked(int button, int state, int x, int y)
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

void VehicleSelection::mouseMoved(int x, int y)
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

void VehicleSelection::mousePassive(int x, int y)
{
}

void VehicleSelection::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	vehicleSelectCam->perspective(60.f, aspect, 1, 1000);
	vehicleSelectCam->update(0);

	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);
	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(w, h);
	}
}

void VehicleSelection::performUpdates(float dt)
{
	for (GameObject* object : updateShip)
	{
		object->update(dt);
	}
}

void VehicleSelection::setUpVehicleSelect()
{	
	for (int i = 0; i < 4; i++)
	{
		backCheckTriggers[2 * i] = 0.f;
		backCheckTriggers[2 * i + 1] = 0.f;
		backCheckButton[2 * i] = false;
		backCheckButton[2 * i + 1] = false;
		playerSelect[i] = i;
		playerReady[i] = false;
		backCheckSticks[2 * i] = { 0, 0 };
		backCheckSticks[2 * i + 1] = { 0, 0 };

		playerStillModel[i].resize(4);

		Player* _P = nullptr;
		GameObject* _PV = nullptr;
		for (int j = 0; j < 4; j++)
		{
			switch (i)
			{
			case 0:
				_P = rm::getCloneOfPlayer("PLAYER_TRUCK");
				break;
			case 1:
				_P = rm::getCloneOfPlayer("PLAYER_TANK");
				break;
			case 2:
				_P = rm::getCloneOfPlayer("PLAYER_BULLDOZER");
				break;
			case 3:
				_P = rm::getCloneOfPlayer("PLAYER_WRECKINGBALL");
				break;
			}

			playerStillModel[i][j] = _P;
		}
		_PV = rm::getCloneOfObject("PURE_EMPTY");

		_PV->setLocalPos(vec3(-3.74f + 2.3f * i, 0.15f, -1.0f));
		_PV->setLocalRotY(180.f);
		_PV->setScale(0.4f);
		_PV->update(0);
		_PV->addChild(_P);
		_P->update(0);

		playerView[i] = _PV;



		vec3 oC;
		switch (i)
		{
		case 0:
			oC = vec3(0.4f, 0.4f, 1.f);
			break;
		case 1:
			oC = vec3(1.f, 0.4f, 0.4f);
			break;
		case 2:
			oC = vec3(0.4f, 1.f, 0.4f);
			break;
		case 3:
			oC = vec3(1.f, 1.f, 0.4f);
			break;
		}

		RecolorObject* _RO = rm::getCloneOfRecolorObject("ARROW_MODEL_LEFT");

		_RO->objectColor = oC;
		_RO->setLocalRotX(180.f);
		_RO->setLocalPos(_PV->getLocalPos() + vec3(-0.4f, 0, 1.f));
		_RO->setScale(0.7f);
		_RO->update(0);
		sceneButtons[2 * i] = _RO;

		_RO = rm::getCloneOfRecolorObject("ARROW_MODEL_RIGHT");

		_RO->objectColor = oC;
		_RO->setLocalPos(_PV->getLocalPos() + vec3(0.4f, 0, 1.f));
		_RO->setScale(0.7f);
		_RO->update(0);
		sceneButtons[2 * i + 1] = _RO;

		Text* _T = rm::getCloneOfText("TextRot");
		_T->setMessage("READY!");
		_T->setScale(0.2f);
		_T->setLocalPos(_PV->getLocalPos() + vec3(0, 1.f, 1.f));
		_T->baseColor = vec3(1.f, 1.f, 1.f);
		for (unsigned int j = 0; j < _T->messageSize(); j++)
		{
			_T->colorShift[j] = vec3(0.f, 1.f, 0.f);
			//_T->posOffset[j] = vec3(0.f);
		}

		_T->update(0);
		READY[i] = _T;
		READY[i]->HIDE = true;

		READY_TIME[i] = 0.f;
	}

	backDrop = rm::getCloneOfObject("SCENE_VEHICLE");
	backDrop->setLocalPos(vec3(0, 0, 0));
	backDrop->update(0);

	//backButton = rm::getCloneOfRecolorObject("BUTTON_BACK");
	//backButton->setLocalPos(vec3(-3.f, 2.5f, 0.f));
	//backButton->setLocalRotY(30.f);
	//backButton->objectColor = vec3(1.f, 0.2f, 0.2f);
	//backButton->update(0);

	tSwitch = rm::getTexture("SCENE_VEHICLE_BEGIN");
	tFade = rm::getTexture("defaultBlack");

	TEXT_BACK = rm::getCloneOfText("TextRot");
	TEXT_BACK->setMessage("BACK-(B) ");
	TEXT_BACK->setLocalPos(vec3(-3.f, 2.5f, 0.f));
	TEXT_BACK->setLocalRotY(30.f);
	TEXT_BACK->setScale(0.3f);
	TEXT_BACK->baseColor = vec3(1.f, 1.f, 1.f);

	SUN = rm::getCloneOfLight("SUNNIER_LIGHT");
	SUN->update(0);
	SUN2 = rm::getCloneOfLight("SUNNIER_LIGHT");
	SUN2->setLocalRotY(180.f);
	SUN2->setLocalRotX(90.f);
	SUN2->update(0);

	vehicleSelectCam = rm::getCloneOfCamera("TITLE_CAM");
	vehicleSelectCam->setLocalPos(vec3(-0.33f, 1.5f, 5.f));
	vehicleSelectCam->giveNewPersRatio(aspect);
	vehicleSelectCam->update(0);

	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(windowWidth, windowHeight);
	}
}
