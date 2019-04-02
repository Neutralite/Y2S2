#include "Scene.h"

int Scene::windowWidth, Scene::windowHeight;
float Scene::aspect;

bool Scene::guiEnabled;

bool Scene::keysDown[256];
bool Scene::backCheckKeysDown[256];
bool Scene::mouseDown[10];
bool Scene::backCheckMouseDown[10];

vec2 Scene::mousePosition, prevMousePosition;
vec2 Scene::DeltaMousePos;

XinputManager Scene::_INPUT;
std::vector<XinputController*> Scene::controllers;
std::vector<Stick> Scene::playerInput;
std::vector<Triggers> Scene::playerTriggers;

UniformBuffer Scene::uniformBufferTime;
UniformBuffer Scene::uRes;

bool Scene::killTheGPUWithLights;

constexpr int Scene::frameTimeNumSamples;
int Scene::frameTimeCurrSample;
float Scene::frameTimeSamples[frameTimeNumSamples];

ShaderProgram* Scene::TEXT_ROT;
ShaderProgram* Scene::TRANSITION_SHADER;
ShaderProgram* Scene::EXPLOSIONSHADER;
ShaderProgram* Scene::MINESHADER;
ShaderProgram* Scene::COMIC_MINE;
ShaderProgram* Scene::COMIC_EXPLOSION;
ShaderProgram* Scene::COMIC_SETUP;
ShaderProgram* Scene::COMIC_EXECUTION;
ShaderProgram* Scene::COMIC_DEFERRED_DIRECTIONAL;
ShaderProgram* Scene::COMIC_DEFERRED_POINT;
ShaderProgram* Scene::COMIC_DEFERRED_SPOTLIGHT;
ShaderProgram* Scene::MESHLIGHT_DEFERRED_DIRECTIONAL;
ShaderProgram* Scene::MESHLIGHT_DEFERRED_POINT;
ShaderProgram* Scene::MESHLIGHT_DEFERRED_SPOTLIGHT;
ShaderProgram* Scene::OUTPUT;
ShaderProgram* Scene::BOUNCE_SETUP;
ShaderProgram* Scene::COMBINED_DRAW;
ShaderProgram* Scene::RADIAL_BLUR;
ShaderProgram* Scene::HORIZONTAL_BLUR;
ShaderProgram* Scene::VERTICAL_BLUR;
ShaderProgram* Scene::BLUR_OUTPUT;
ShaderProgram* Scene::TEXT_SHADER;
ShaderProgram* Scene::TEXT_UI;
ShaderProgram* Scene::DOUTPUT;
ShaderProgram* Scene::BLACKOUT;
ShaderProgram* Scene::LUT;

Framebuffer* Scene::sceneCapture;
Framebuffer* Scene::collect;
Framebuffer* Scene::defLight;
Framebuffer* Scene::transition;
Framebuffer* Scene::UI_SCREEN;
Framebuffer* Scene::SHADOW_FB;

Texture* Scene::overlay;
Texture* Scene::tRamp;
Texture* Scene::tDiffuse;
Texture* Scene::difOver;
Texture* Scene::transOver;
Texture* Scene::BoringRamp;
Texture* Scene::SteepRamp;
Texture* Scene::SuddenRamp;
Texture* Scene::TwoStopRamp;

Sound* Scene::backgroundMusic;
FMOD::Channel* Scene::backgroundChannel;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::setDefaults()
{
	TRANSITION_SHADER = rm::getShader("TRANSITION");
	EXPLOSIONSHADER = rm::getShader("EXPLOSION");
	MINESHADER = rm::getShader("MINES");
	COMIC_MINE = rm::getShader("COMIC_MINE");
	COMIC_EXPLOSION = rm::getShader("COMIC_EXPLOSION");
	COMIC_SETUP = rm::getShader("COMIC_SETUP");
	COMIC_EXECUTION = rm::getShader("COMIC_EXECUTION");
	COMIC_DEFERRED_DIRECTIONAL = rm::getShader("COMIC_DEFERRED_DIRECTIONAL");
	COMIC_DEFERRED_POINT = rm::getShader("COMIC_DEFERRED_POINT");
	COMIC_DEFERRED_SPOTLIGHT = rm::getShader("COMIC_DEFERRED_SPOTLIGHT");
	MESHLIGHT_DEFERRED_DIRECTIONAL = rm::getShader("LIGHTMESH_DEFERRED_DIRECTIONAL");
	MESHLIGHT_DEFERRED_POINT = rm::getShader("LIGHTMESH_DEFERRED_POINT");
	MESHLIGHT_DEFERRED_SPOTLIGHT = rm::getShader("LIGHTMESH_DEFERRED_SPOTLIGHT");
	OUTPUT = rm::getShader("JUST_OUTPUT");
	BOUNCE_SETUP = rm::getShader("BOBBING_SETUP");
	COMBINED_DRAW = rm::getShader("COMBINED_DRAW_SHADER");
	RADIAL_BLUR = rm::getShader("RADIAL_BLUR");
	HORIZONTAL_BLUR = rm::getShader("BLUR_HORIZONTAL");
	VERTICAL_BLUR = rm::getShader("BLUR_VERTICAL");
	BLUR_OUTPUT = rm::getShader("BLUR_EMISSIVE_OUTPUT");
	TEXT_SHADER = rm::getShader("TEXT_SHADER");
	TEXT_ROT = rm::getShader("TEXT_ROT_SHADER");
	TEXT_UI = rm::getShader("TEXT_UI");
	DOUTPUT = rm::getShader("DEPTH_CHECK_OUTPUT");
	BLACKOUT = rm::getShader("PER_PLAYER_TRANS");
	LUT = rm::getShader("LUT");

	sceneCapture = rm::getFramebuffer("INITIAL_SCREEN");
	defLight = rm::getFramebuffer("DEF_LIGHT");
	transition = rm::getFramebuffer("TRANSITION");
	UI_SCREEN = rm::getFramebuffer("UI_SCREEN");
	collect = rm::getFramebuffer("COLLECT");
	SHADOW_FB = rm::getFramebuffer("SHADOW_FB");

	SHADOW_FB->reshape(1024, 1024);
	SHADOW_FB->isFixedSize = true;

	overlay = rm::getTexture("Medium shading");
	tRamp = rm::getTexture("Game Toon Ramp");
	tDiffuse = rm::getTexture("Diffuse Toon");
	tDiffuse->setWrapParameters(GL_CLAMP_TO_EDGE);
	tDiffuse->sendTexParameters();
	BoringRamp = rm::getTexture("BoringRamp");
	BoringRamp->setWrapParameters(GL_CLAMP_TO_EDGE);
	BoringRamp->sendTexParameters();
	SteepRamp = rm::getTexture("SteepRamp");
	SteepRamp->setWrapParameters(GL_CLAMP_TO_EDGE);
	SteepRamp->sendTexParameters();
	SuddenRamp = rm::getTexture("SuddenDrop");
	SuddenRamp->setWrapParameters(GL_CLAMP_TO_EDGE);
	SuddenRamp->sendTexParameters();
	TwoStopRamp = rm::getTexture("TwoStopRamp");
	TwoStopRamp->setWrapParameters(GL_CLAMP_TO_EDGE);
	TwoStopRamp->sendTexParameters();
	difOver = rm::getTexture("Diffuse Overlay");

	transOver = rm::getTexture("defaultBlack");

	killTheGPUWithLights = false;

	backgroundMusic = nullptr;
	backgroundChannel = nullptr;
}

void Scene::setName(std::string _NAME)
{
	name = _NAME;
}

std::string Scene::getName()
{
	return name;
}

void Scene::pause(bool _P)
{
	paused = _P;
}

bool Scene::isPaused()
{
	return paused;
}

void Scene::drawChildren(std::vector<Transform*>* drawList, std::vector<Light*> *lightList, Transform * TF, bool doLights)
{
	if (!TF->HIDE)
	{
		bool keepLight = doLights;
		if (TF->destroying)
		{
			keepLight = false;
		}
		switch (TF->TT)
		{
		case Transform::TransformType::TYPE_Powerup:
			//std::cout << "ONE IS THERE!" << std::endl;
		case Transform::TransformType::TYPE_GameObject:
		case Transform::TransformType::TYPE_BasePlate:
		case Transform::TransformType::TYPE_Destructable:
		case Transform::TransformType::TYPE_Player:
		case Transform::TransformType::TYPE_Text:
		case Transform::TransformType::TYPE_Mine:
		case Transform::TransformType::TYPE_Hammer:
		case Transform::TransformType::TYPE_Axe:
		case Transform::TransformType::TYPE_Recolor:

			drawList->push_back(TF);
			break;
		case Transform::TransformType::TYPE_Light:
			if (keepLight)
			{
				Light* LIT = dynamic_cast<Light*>(TF);
				//lightShip.push_back(LIT);
				protectedEntityShip(lightList, LIT);
			}
			break;
		}

		for (Transform* TF2 : TF->getChildren())
		{
			//std::cout << TF->getChildren().size() << std::endl;
			drawChildren(drawList, lightList, TF2, keepLight);
		}
	}
}

void Scene::UIDrawChildren(std::vector<Transform*>* drawList, Transform * TF)
{
	if (!TF->HIDE)
	{
		switch (TF->TT)
		{
		case Transform::TransformType::TYPE_Powerup:
		case Transform::TransformType::TYPE_GameObject:
		case Transform::TransformType::TYPE_BasePlate:
		case Transform::TransformType::TYPE_Destructable:
		case Transform::TransformType::TYPE_Player:
		case Transform::TransformType::TYPE_Text:
		case Transform::TransformType::TYPE_Mine:
		case Transform::TransformType::TYPE_Hammer:
		case Transform::TransformType::TYPE_Axe:
		case Transform::TransformType::TYPE_Recolor:
			drawList->push_back(TF);
			break;
		}

		for (Transform* TF2 : TF->getChildren())
		{
			//std::cout << TF->getChildren().size() << std::endl;
			UIDrawChildren(drawList, TF2);
		}
	}
}

void Scene::drawFromLight(std::vector<Transform*>* drawList, Transform * TF)
{
	if (!TF->HIDE)
	{
		switch (TF->TT)
		{
		case Transform::TransformType::TYPE_GameObject:
		case Transform::TransformType::TYPE_BasePlate:
		case Transform::TransformType::TYPE_Destructable:
		case Transform::TransformType::TYPE_Player:
		case Transform::TransformType::TYPE_Mine:
		case Transform::TransformType::TYPE_Hammer:
		case Transform::TransformType::TYPE_Axe:
		case Transform::TransformType::TYPE_Recolor:
			drawList->push_back(TF);
			break;
		}

		for (Transform* TF2 : TF->getChildren())
		{
			//std::cout << TF->getChildren().size() << std::endl;
			UIDrawChildren(drawList, TF2);
		}
	}
}



void Scene::setKeysDown(bool down, unsigned char key)
{
	keysDown[key] = down;
}

void Scene::keyHandler()
{
	for (int i = 0; i < 256; i++)
		backCheckKeysDown[i] = keysDown[i];
}

void Scene::mouseHandler()
{
	for (int i = 0; i < 10; i++)
		backCheckMouseDown[i] = mouseDown[i];
}

void Scene::triggerHandler()
{
}
