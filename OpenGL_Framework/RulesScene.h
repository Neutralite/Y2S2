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

class Rules : public Scene
{
public:
	Rules();
	~Rules();

	void initializeGame();
	void update();
	void draw();
	void GUI();
	void releaseScene();

	void beginningUpdate(float dt);
	void startingUpdate(float dt);
	void runningUpdate(float dt);
	void enterUpdate(float dt);
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

	void setUpRules();

	float TotalGameTime = 0.0f;
	float deltaTime = 0.f;
	float tTime = 1.0001f;
private:
	Timer *updateTimer = nullptr;

	Light* SUN = nullptr;
	GameObject* _RULES = nullptr;
	Text* BACK_B = nullptr;
	Camera* RULES_CAM = nullptr;

	Texture* tSwitch = nullptr;
	Texture* tFade = nullptr;

	std::vector<Transform*> renderShip;
	std::vector<Transform*> UIRenderShip;
	std::vector<GameObject*> updateShip;
	std::vector<Light*> lightShip;

	enum gameState
	{
		GS_BEGINNING,
		GS_STARTING,
		GS_RUNNING,
		GS_ENDED,
		GS_EXITING
	};

	gameState _GS = GS_BEGINNING;

	bool goingBack = false;
	float backFloat = 0.f;
};