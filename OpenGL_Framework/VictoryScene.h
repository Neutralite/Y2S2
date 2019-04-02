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

class VictoryScene : public Scene
{
public:
	VictoryScene();
	~VictoryScene();

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

	void setUpVictory();

	float TotalGameTime = 0.0f;
	float deltaTime = 0.f;
	float tTime = 1.0001f;
	float holdOff = 0.f;
	float sinWaveTime = 0.f;
	float fadeOutTime = 0.f;
	float justBecauseItFeelsNice = 0.f;
private:
	Timer *updateTimer = nullptr;

	Camera* victoryCam = nullptr;
	GameObject* EMPTY = nullptr;

	RecolorObject* backdrop = nullptr;

	//std::vector<GameObject*> playerModels;
	std::vector<float> scoreLerp;
	std::vector<RecolorObject*> playerScoreBars;
	std::vector<Player*> playerModels;
	std::vector<Text*> playerScores;
	std::vector<Text*> playerScoresBACK;
	std::vector<std::string> playerNames;

	std::vector<int> preScores;
	std::vector<std::string> preNames;
	std::vector<Text*> preText;
	std::vector<int> editSlot;

	RecolorObject* U_A = nullptr;
	RecolorObject* D_A = nullptr;

	Text* nameInitials = nullptr;
	Text* confirmation = nullptr;

	Light* SUN = nullptr;

	Texture* tSwitch = nullptr;
	Texture* tFade = nullptr;

	std::vector<float> backCheckTriggers;
	std::vector<bool> backCheckButton;
	std::vector<vec2> backCheckSticks;

	int maxScore = 0;
	int enteringPlayer = 0;
	int letterSel = 0;
	int leaderboarders = 0;
	int accum = 0;

	char L1 = 'A';
	char L2 = 'A';
	char L3 = 'A';

	std::vector<Transform*> renderShip;
	std::vector<Transform*> UIRenderShip;
	std::vector<GameObject*> updateShip;
	std::vector<Light*> lightShip;

	enum gameState
	{
		GS_BEGINNING,
		GS_STARTING,
		GS_RUNNING,
		GS_ENTER,
		GS_ENDED,
		GS_EXITING
	};

	bool runThrough = false;

	gameState _GS = GS_BEGINNING;
};
