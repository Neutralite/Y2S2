#include "RulesScene.h"

Rules::Rules()
{
	updateTimer = new Timer();
}

Rules::~Rules()
{
	delete updateTimer;
}

void Rules::initializeGame()
{
	readyToTerminate = false;
	TotalTime = 0.f;
	tTime = 1.0001f;
	TotalGameTime = 0.f;
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	setUpRules();
	
	goingBack = false;
	backFloat = 0.f;

	_GS = GS_BEGINNING;

	backgroundMusic->Stop(backgroundChannel);
	backgroundMusic = rm::getSound("DRUMZ");
	backgroundChannel = backgroundMusic->Play();
	backgroundChannel->setVolume(0.2f);
}

void Rules::update()
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
}

void Rules::draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (Framebuffer* _FB : ResourceManager::allFramebuffers)
		_FB->clear();

	RULES_CAM->sendUBO();
	RULES_CAM->setRenderList(renderShip);
	RULES_CAM->cull();
	RULES_CAM->render();

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

void Rules::GUI()
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

	vec3 cPos = RULES_CAM->getLocalPos();
	ImGui::SliderFloat3("CAMERA_POSITION", &cPos.x, -10, 10, "%.5f");
	RULES_CAM->setLocalPos(cPos);

	ImGui::End();

	UI::End();
}

void Rules::releaseScene()
{
	rm::order66();
}

void Rules::beginningUpdate(float dt)
{
	tTime -= deltaTime * 0.8f;
	//std::cout << tTime << std::endl;
	if (tTime < 0.f)
	{
		_GS = GS_STARTING;
		tTime = 0.f;
	}

	drawChildren(&renderShip, &lightShip, _RULES, false);
	drawChildren(&renderShip, &lightShip, BACK_B, false);

	protectedEntityShip(&lightShip, SUN);
	uniformBufferTime.sendFloat(TotalGameTime, 0);
}

void Rules::startingUpdate(float dt)
{
	_GS = GS_RUNNING;
	drawChildren(&renderShip, &lightShip, _RULES, false);
	drawChildren(&renderShip, &lightShip, BACK_B, false);

	protectedEntityShip(&lightShip, SUN);
	uniformBufferTime.sendFloat(TotalGameTime, 0);
}

void Rules::runningUpdate(float dt)
{
	goingBack = false;

	for (int i = 0; i < 4; i++)
	{
		if (controllers[i])
		{
			if (controllers[i]->isButtonPressed(B))
			{
				goingBack = true;
			}
		}
	}

	if (keysDown['q'])
	{
		goingBack = true;
	}
	
	float mAm = backFloat * BACK_B->messageSize();

	for (unsigned int i = 0; i < BACK_B->messageSize(); i++)
	{
		if (mAm >= i + 1)
		{
			BACK_B->colorShift[i] = vec3(1.f, 0.1f, 0.1f);
			BACK_B->tS[i] = vec3(1.3f);
		}
		else
		{
			BACK_B->colorShift[i] = vec3(1.f, 1.f, 1.f);
			BACK_B->tS[i] = vec3(1.f);
		}
	}

	drawChildren(&renderShip, &lightShip, _RULES, false);
	drawChildren(&renderShip, &lightShip, BACK_B, false);

	protectedEntityShip(&lightShip, SUN);
	uniformBufferTime.sendFloat(TotalGameTime, 0);

	if (goingBack)
	{
		backFloat += deltaTime * 0.5f;
		if (backFloat > 1.f)
		{
			_GS = GS_ENDED;
		}
	}
	else
	{
		backFloat = 0.f;
	}
}

void Rules::endedUpdate(float dt)
{
	tTime += deltaTime * 0.8f;
	//std::cout << tTime << std::endl;
	if (tTime > 1.f)
	{
		_GS = GS_EXITING;
		tTime = 1.f;
	}

	drawChildren(&renderShip, &lightShip, _RULES, false);
	drawChildren(&renderShip, &lightShip, BACK_B, false);

	protectedEntityShip(&lightShip, SUN);
	uniformBufferTime.sendFloat(TotalGameTime, 0);
}

void Rules::exitingUpdate(float dt)
{
	readyToTerminate = true;
	nextScene = "TITLE_SCREEN";
}

void Rules::keyboardDown(unsigned char key, int mouseX, int mouseY)
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

void Rules::keyboardUp(unsigned char key, int mouseX, int mouseY)
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

void Rules::keyboardSpecialDown(int key, int mouseX, int mouseY)
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

void Rules::keyboardSpecialUp(int key, int mouseX, int mouseY)
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

void Rules::mouseClicked(int button, int state, int x, int y)
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

void Rules::mouseMoved(int x, int y)
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

void Rules::mousePassive(int x, int y)
{
}

void Rules::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	RULES_CAM->giveNewOrthoRatio(aspect);

	_RULES->setScale(vec3(RULES_CAM->m_pOrthoSize.y, 0, RULES_CAM->m_pOrthoSize.w)  * 2.f / 6.f);
	_RULES->update(0);

	BACK_B->setLocalPos(vec3(RULES_CAM->m_pOrthoSize.y, RULES_CAM->m_pOrthoSize.z, 1) + vec3(-4 - BACK_B->wordLength * 0.5f, 4, 0) * BACK_B->aS);
	BACK_B->update(0);

	vec4 RES = vec4((float)w, (float)h, 1.f / (float)w, 1.f / (float)h);
	uRes.sendVector(RES, 0);
	glViewport(0, 0, w, h);

	for (Framebuffer* FB : ResourceManager::allFramebuffers)
	{
		if (!FB->isFixedSize)
			FB->reshape(w, h);
	}
}

void Rules::performUpdates(float dt)
{
	for (GameObject* object : updateShip)
	{
		object->update(dt);
	}
}

void Rules::setUpRules()
{
	RULES_CAM = rm::getCloneOfCamera("VICTORY_CAM");
	RULES_CAM->giveNewOrthoRatio(aspect);
	RULES_CAM->update(0);

	SUN = rm::getCloneOfLight("HEAD_ON_SUN");
	SUN->update(0);

	BACK_B = rm::getCloneOfText("TextRot");
	BACK_B->setMessage("BACK (HOLD B) ");
	BACK_B->aS = vec3(0.4f);
	BACK_B->setLocalPos(vec3(RULES_CAM->m_pOrthoSize.y, RULES_CAM->m_pOrthoSize.z, 1) + vec3(-4 - BACK_B->wordLength * 0.5f, 4, 0) * BACK_B->aS);
	BACK_B->baseColor = vec3(1.f);
	for (unsigned int j = 0; j < BACK_B->messageSize(); j++)
	{
		BACK_B->colorShift[j] = vec3(1.f);
	}
	BACK_B->update(0);

	_RULES = rm::getCloneOfObject("GAME_RULES");
	_RULES->setScale(vec3(RULES_CAM->m_pOrthoSize.y, 0, RULES_CAM->m_pOrthoSize.w)  * 2.f / 6.f);
	_RULES->setLocalRotX(90.f);
	_RULES->update(0);

	tSwitch = rm::getTexture("SCENE_RULES_TRANS");
	tFade = rm::getTexture("defaultBlack");
}
