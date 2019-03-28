#pragma once
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Scene.h"
#include "ResourceManager.h"

class Title : public Scene
{
public:
	Title();
	~Title();

	void initializeGame();
	void update();
	void draw();
	void GUI();
	void releaseScene();

	void beginningUpdate(float dt);
	void startingUpdate(float dt);
	void runningUpdate(float dt);
	void endedUpdate(float dt);
	void exitingUpdate(float dt);

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void keyboardSpecialUp(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);
	void mousePassive(int x, int y);
	void reshapeWindow(int w, int h);

	void performUpdates(float dt);

	void setUpTitle();

	float TotalGameTime = 0.0f;
	float deltaTime = 0.f;
	float tTime = 1.0001f;
	float flyOut = 0.f;
	float keyPos1 = 0.6f;
	float keyPos2 = 1.4f;

	int repScene = 0;
private:
	Timer *updateTimer = nullptr;

	Camera* titleCam = nullptr;

	RecolorObject* START = nullptr;
	float StartSize = 1.f;
	RecolorObject* RULES = nullptr;
	float RulesSize = 1.f;
	RecolorObject* EXIT = nullptr;
	float ExitSize = 1.f;
	RecolorObject* ARROW = nullptr;

	GameObject* backdrop = nullptr;

	Destructable* building1 = nullptr;
	Destructable* building2 = nullptr;
	Destructable* tree = nullptr;

	Player* playerModel = nullptr;

	Light* SUN;
	Light* SUN2;

	Texture* tSwitch = nullptr;
	Texture* tFade = nullptr;

	Text* DOWN_TOWN = nullptr;
	Text* DESTRUCTION = nullptr;

	std::vector<float> backCheckTriggers;
	std::vector<bool> backCheckButton;
	std::vector<vec2> backCheckSticks;

	int menuSelection = 0;

	std::vector<Transform*> renderShip;
	std::vector<Transform*> UIRenderShip;
	std::vector<GameObject*> updateShip;
	std::vector<Light*> lightShip;

	float curRotZ;
	vec3 curPos;
	//float flyUp = 1.f;

	enum gameState
	{
		GS_BEGINNING,
		GS_STARTING,
		GS_RUNNING,
		GS_ENDED,
		GS_EXITING
	};

	gameState _GS = GS_BEGINNING;
};
