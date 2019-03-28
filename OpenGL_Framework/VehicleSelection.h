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

class VehicleSelection : public Scene
{
public:
	VehicleSelection();
	~VehicleSelection();

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

	void setUpVehicleSelect();

	float TotalGameTime = 0.0f;
	float deltaTime = 0.f;
	float tTime = 0.f;

	int repScene = 0;
private:
	Timer *updateTimer = nullptr;

	Camera* vehicleSelectCam = nullptr;

	std::vector<std::vector<Player*>> playerStillModel;
	std::vector<GameObject*> playerView;
	std::vector<int> playerSelect;
	std::vector<bool> playerReady;
	std::vector<RecolorObject*> sceneButtons;
	std::vector<Text*> READY;
	std::vector<float> READY_TIME;
	std::vector<float> backCheckTriggers;
	std::vector<bool> backCheckButton;

	GameObject* backDrop = nullptr;
	//RecolorObject* backButton = nullptr;

	Texture* tSwitch = nullptr;
	Texture* tFade = nullptr;

	Light* SUN;
	Light* SUN2;
	Text* TEXT_BACK;

	std::vector<Transform*> renderShip;
	std::vector<Transform*> UIRenderShip;
	std::vector<GameObject*> updateShip;
	std::vector<Light*> lightShip;

	float backTimer = 0.f;

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
