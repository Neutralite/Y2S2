#pragma once
#define NOMINMAX

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"

#include <string>
#include "ResourceManager.h"
#include "UI.h"
#include "XinputManager.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	static void setDefaults();

	void setName(std::string _NAME);
	std::string getName();
	void pause(bool _P);
	bool isPaused();

	virtual void initializeGame() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void releaseScene() = 0;
	//void GUI();

	virtual void keyboardDown(unsigned char key, int mouseX, int mouseY) = 0;
	virtual void keyboardUp(unsigned char key, int mouseX, int mouseY) = 0;
	virtual void keyboardSpecialDown(int key, int mouseX, int mouseY) = 0;
	virtual void keyboardSpecialUp(int key, int mouseX, int mouseY) = 0;
	virtual void mouseClicked(int button, int state, int x, int y) = 0;
	virtual void mouseMoved(int x, int y) = 0;
	virtual void mousePassive(int x, int y) = 0;
	virtual void reshapeWindow(int w, int h) = 0;

	static void drawChildren(std::vector<Transform*> *drawList, std::vector<Light*> *lightList, Transform* TF, bool doLights);
	static void UIDrawChildren(std::vector<Transform*> *drawList, Transform* TF);
	static void drawFromLight(std::vector<Transform*> *drawList, Transform* TF);

	template <class T>
	static void protectedEntityShip(std::vector<T*> *entityList, T* entity);

	template <class T>
	static void protectedAddEntity(std::vector<T*> *entityList, T* entity, unsigned int front, unsigned int back);

	static void setKeysDown(bool down, unsigned char key);
	static void keyHandler();
	static void mouseHandler();
	static void triggerHandler();

	static int windowWidth, windowHeight;
	static float aspect;

	static bool guiEnabled;
	bool readyToTerminate = false;

	static XinputManager _INPUT;

	static std::vector<XinputController*> controllers;
	static std::vector<Stick> playerInput;
	static std::vector<Triggers> playerTriggers;

	static UniformBuffer uniformBufferTime;
	static UniformBuffer uRes;

	static constexpr int frameTimeNumSamples = 600;
	static int frameTimeCurrSample;
	static float frameTimeSamples[frameTimeNumSamples];

	std::string nextScene;
protected:
	static bool keysDown[256];
	static bool backCheckKeysDown[256];
	static bool mouseDown[10];
	static bool backCheckMouseDown[10];

	static vec2 mousePosition, prevMousePosition;
	static vec2 DeltaMousePos;

	std::string name;
	bool paused = false;
	float TotalTime = 0.f;

	//Anything that won't change in between scenes

	static ShaderProgram* TRANSITION_SHADER;
	static ShaderProgram* EXPLOSIONSHADER;
	static ShaderProgram* MINESHADER;
	static ShaderProgram* COMIC_MINE;
	static ShaderProgram* COMIC_EXPLOSION;
	static ShaderProgram* COMIC_SETUP;
	static ShaderProgram* COMIC_EXECUTION;
	static ShaderProgram* COMIC_DEFERRED_DIRECTIONAL;
	static ShaderProgram* COMIC_DEFERRED_POINT;
	static ShaderProgram* COMIC_DEFERRED_SPOTLIGHT;
	static ShaderProgram* MESHLIGHT_DEFERRED_DIRECTIONAL;
	static ShaderProgram* MESHLIGHT_DEFERRED_POINT;
	static ShaderProgram* MESHLIGHT_DEFERRED_SPOTLIGHT;
	static ShaderProgram* OUTPUT;
	static ShaderProgram* BOUNCE_SETUP;
	static ShaderProgram* COMBINED_DRAW;
	static ShaderProgram* RADIAL_BLUR;
	static ShaderProgram* HORIZONTAL_BLUR;
	static ShaderProgram* VERTICAL_BLUR;
	static ShaderProgram* BLUR_OUTPUT;
	static ShaderProgram* TEXT_SHADER;
	static ShaderProgram* TEXT_ROT;
	static ShaderProgram* TEXT_UI;
	static ShaderProgram* DOUTPUT;
	static ShaderProgram* BLACKOUT;

	static Framebuffer* sceneCapture;
	static Framebuffer* collect;
	static Framebuffer* defLight;
	static Framebuffer* transition;
	static Framebuffer* UI_SCREEN;
	static Framebuffer* SHADOW_FB;

	static Texture* overlay;
	static Texture* tRamp;
	static Texture* tDiffuse;
	static Texture* difOver;
	static Texture* transOver;
};

template<class T>
inline void Scene::protectedEntityShip(std::vector<T*>* entityList, T * entity)
{
	if (entityList->size() > 0)
		protectedAddEntity(entityList, entity, 0, entityList->size() - 1);
	else
		entityList->push_back(entity);
}

template<class T>
inline void Scene::protectedAddEntity(std::vector<T*>* entityList, T * entity, unsigned int front, unsigned int back)
{
	unsigned int mid = (front + back) / 2;
	if ((*entityList)[mid] == entity)
	{

	}
	else if (back - front <= 1)
	{
		if ((*entityList)[front] == entity || (*entityList)[back] == entity)
		{

		}
		else if ((*entityList)[front] > entity)
			entityList->insert(entityList->begin() + front, entity);
		else if ((*entityList)[back] > entity)
			entityList->insert(entityList->begin() + back, entity);
		else
			entityList->insert(entityList->begin() + back + 1, entity);
	}
	else if ((*entityList)[mid] > entity)
	{
		protectedAddEntity(entityList, entity, front, mid);
	}
	else
	{
		protectedAddEntity(entityList, entity, mid, back);
	}
}
