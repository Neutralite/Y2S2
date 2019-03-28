#pragma once
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "IO.h"
#include "GameObject.h"
#include "UniformBuffer.h"
#include "Light.h"
#include "Framebuffer.h"
#include "PostProcessBuffer.h"

#include "BasePlate.h"
#include "Boundary.h"
#include "Destructable.h"
#include "Player.h"
#include "HITBOX2D.h"
#include "PhysicsBody.h"

#include "Field.h"
#include "XinputManager.h"
#include "Scene.h"

#define WINDOW_SCREEN_WIDTH		640
#define WINDOW_SCREEN_HEIGHT	432
#define WINDOW_WIDTH			960
#define WINDOW_HEIGHT			720
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class Game : public Scene
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();
	void GUI();
	void releaseScene();

	void beginningUpdate(float dt);
	void startingUpdate(float dt);
	void runningUpdate(float dt);
	void pausedUpdate(float dt);
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

	void updateExternals(float dt);
	void addToCollisions(GameObject* GO, bool dynamic);
	void putInStatic(GameObject* _GO, unsigned int front, unsigned int back);
	void putInDynamic(GameObject* _GO, unsigned int front, unsigned int back);
	void staticCollisions();
	void batchMesh(GameObject * _GO, unsigned int front, unsigned int back);
	void protectedBatch(GameObject* _GO);
	void protectedWeaponShip(Weapon* _W);
	void protectedAddWeapon(Weapon* _W, unsigned int front, unsigned int back);

	void generateATTACK(Player* P);
	void updateAttacks(float dt);
	void attackHIT(unsigned int index);

	void allSetup();

	void setBaseAndBoundaries();
	void setShaders();
	void setFramebuffers();
	void setCamerasAndPlayers();
	void generateMap();

	void performUpdates(float dt);
	void updateSingle(float dt, GameObject* _T);

	void uniqueKeyPresses();
	void resetMap();

	/* Data Members */
	Timer *updateTimer	= nullptr;
	Timer *gameCheckTimer = nullptr;

	float TotalGameTime = 0.0f;
	float maxGameTimer = 180.f;
	float tTime = 1.0001f;

	bool getTime = false;

	bool isInScene = false;

	unsigned int gameFrame = 0;

	float deltaTime = 0.f;
	float dropOffTimer = 0.f;
private:
	float tileSize = 6.f;

	Boundary* RIGHT_WALL;
	Boundary* LEFT_WALL;
	Boundary* UPPER_WALL;
	Boundary* LOWER_WALL;

	BasePlate* BASE_PLATE_SAND;
	BasePlate* BASE_PLATE_DIRT;
	BasePlate* BASE_PLATE_CONCRETE;
	BasePlate* BASE_PLATE_GRASS;
	BasePlate* BASE_PLATE_ROAD;
	BasePlate* BASE_PLATE_4WAY;
	BasePlate* BASE_PLATE_T;
	BasePlate* BASE_PLATE_CORNER;
	Light* SUN;

	Camera* PlayerCam;
	Camera* UIcam;
	std::vector<Player*> players;

	Field* theMap;
	std::vector<Transform*> renderShip;
	std::vector<Transform*> UIRenderShip;
	std::vector<GameObject*> updateShip;
	std::vector<GameObject*> externalUpdateShip;
	std::vector<GameObject*> dynamicCollisionShip;
	std::vector<GameObject*> staticCollisionShip;
	std::vector<GameObject*> RE_SPAWN;
	std::vector<Light*> lightShip;
	std::vector<GameObject*> shadowShip;
	std::vector<Mesh*> dynamicBatchShip;
	std::vector<Weapon*> weaponShip;
	std::vector<Text*> textShip;
	std::vector<Text*> UITextShip;

	std::vector<Text*> pTotals;
	std::vector<float> pFloats;
	float pMax = 1.0f;
	Text* TUI;
	Text* TUI2;

	PostProcessBuffer RADIAL_POST_PROC;

	bool useFirst = false;
	float radialBlur = 0.f;
	int radialLoops = 3;

	int pScore = 0;
	int lerpScore = 0;

	vec3 rShift;
	vec3 gShift;
	vec3 bShift;
	float shiftAmnt;
	int bloomHeight;
	int radialHeight;

	int minutes = 3;
	float seconds = 0;
	std::string timeString;
	Text* TIMER;
	Text* TIMER2;

	Texture* tSwitch = nullptr;
	Texture* tFade = nullptr;

	enum gameState
	{
		GS_BEGINNING,
		GS_STARTING,
		GS_RUNNING,
		GS_PAUSED,
		GS_ENDED,
		GS_EXITING
	};

	gameState _GS = GS_BEGINNING;
};
