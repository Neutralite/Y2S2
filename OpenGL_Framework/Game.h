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

#include "BasePlate.h"
#include "Boundary.h"
#include "Destructable.h"
#include "Player.h"
#include "HITBOX2D.h"
#include "PhysicsBody.h"

#include "Field.h"
#include "XinputManager.h"

#define WINDOW_SCREEN_WIDTH		640
#define WINDOW_SCREEN_HEIGHT	432
#define WINDOW_WIDTH			960
#define WINDOW_HEIGHT			720
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();
	void GUI();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void keyboardSpecialUp(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);
	void mousePassive(int x, int y);
	void reshapeWindow(int w, int h);

	void setKeysDown(bool down, unsigned char key);
	void keyHandler();
	void mouseHandler();
	void triggerHandler();
	void drawChildren(Transform* TF);

	void protectedUpdateShip(GameObject* GO);
	void protectedExternalUpdateShip(GameObject* GO);
	void protectedAddUpdate(GameObject* _GO, unsigned int front, unsigned int back);
	void protectedAddExternalUpdate(GameObject* _GO, unsigned int front, unsigned int back);
	void protectedLightShip(Light* LIT);
	void protectedAddLight(Light* LIT, unsigned int front, unsigned int back);
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

	void loadAllTextures(std::string &fileName);
	void loadAllMeshes(std::string &fileName);
	void loadAllObjects(std::string &fileName);
	void loadAllUIElements(std::string &fileName);
	void loadAllLights(std::string &filename);
	void loadAllHitboxes(std::string &fileName);
	void loadAllPlayerObjects(std::string &fileName);
	void loadAllMorphTargs(std::string &fileName);
	void loadAllShaders(std::string &fileName);
	void loadAllFramebuffers(std::string &fileName);
	void loadAllCameras(std::string &fileName);

	void createChild(std::string &fileName, Transform* parent);
	void createUniqueChild(std::string &fileName, Transform* parent);

	void allSetup();

	void setBaseAndBoundaries();
	void setShaders();
	void setFramebuffers();
	void setCamerasAndPlayers();
	void generateMap();

	void performUpdates(float dt);
	void updateSingle(float dt, GameObject* _T);
	
	Boundary* getBoundary(std::string _NAME);
	Destructable* getDestructable(std::string _NAME);
	Player* getPlayer(std::string _NAME);
	BasePlate* getBasePlate(std::string _NAME);
	Light* getLight(std::string _NAME);
	Hitbox2D* getHitbox(std::string _NAME);
	Mesh* getMesh(std::string _NAME);
	Texture* getTexture(std::string _NAME);
	Camera* getCamera(std::string _NAME);
	Transform* getEntity(std::string _NAME);
	ShaderProgram* getShader(std::string _NAME);
	GameObject* getObject(std::string _NAME);
	Framebuffer* getFramebuffer(std::string _NAME);

	Boundary* getCloneOfBoundary(std::string _NAME);
	Destructable* getCloneOfDestructable(std::string _NAME);
	Player* getCloneOfPlayer(std::string _NAME);
	BasePlate* getCloneOfBasePlate(std::string _NAME);
	Light* getCloneOfLight(std::string _NAME);
	Camera* getCloneOfCamera(std::string _NAME);
	Transform* getCloneOfEntity(std::string _NAME);
	GameObject* getCloneOfObject(std::string _NAME);

	void cloneChildren(Transform* _TF);

	void uniqueKeyPresses();
	void resetMap();

	/* Data Members */
	Timer *updateTimer	= nullptr;
	Timer *gameCheckTimer = nullptr;

	float TotalGameTime = 0.0f;
	int windowWidth = WINDOW_WIDTH;
	int windowHeight = WINDOW_HEIGHT;

	bool paused = false;
	bool getTime = false;

	unsigned int gameFrame = 0;

private:
	float tileSize = 6.f;

	bool keysDown[256];
	bool backCheckKeysDown[256];
	bool mouseDown[10];
	bool backCheckMouseDown[10];

	vec2 mousePosition, prevMousePosition;
	vec2 DeltaMousePos;

	float aspect;

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
	Texture* overlay;
	Texture* tRamp;

	//ShaderProgram* PassThrough;
	ShaderProgram* EXPLOSIONSHADER;
	ShaderProgram* MINESHADER;
	ShaderProgram* COMIC_MINE;
	ShaderProgram* COMIC_EXPLOSION;
	ShaderProgram* COMIC_SETUP;
	ShaderProgram* COMIC_EXECUTION;
	ShaderProgram* COMIC_DEFERRED_DIRECTIONAL;
	ShaderProgram* COMIC_DEFERRED_POINT;
	ShaderProgram* COMIC_DEFERRED_SPOTLIGHT;
	ShaderProgram* MESHLIGHT_DEFERRED_DIRECTIONAL;
	ShaderProgram* MESHLIGHT_DEFERRED_POINT;
	ShaderProgram* MESHLIGHT_DEFERRED_SPOTLIGHT;
	ShaderProgram* OUTPUT;
	ShaderProgram* BOUNCE_SETUP;

	Framebuffer* sceneCapture;
	Framebuffer* defLight, *defLight2;

	Camera* PlayerCam;
	std::vector<Player*> players;

	Field* theMap;
	std::vector<Transform*> renderShip;
	std::vector<GameObject*> updateShip;
	std::vector<GameObject*> externalUpdateShip;
	std::vector<GameObject*> dynamicCollisionShip;
	std::vector<GameObject*> staticCollisionShip;
	std::vector<Light*> lightShip;
	std::vector<GameObject*> shadowShip;
	std::vector<Mesh*> dynamicBatchShip;
	std::vector<Weapon*> weaponShip;

	UniformBuffer uniformBufferTime;
	UniformBuffer uRes;

	XinputManager _INPUT;
	std::vector<XinputController*> controllers;
	std::vector<Stick> playerInput;
	std::vector<Triggers> playerTriggers;

	bool useFirst = false;

	// Scene Objects.
	//Camera camera;
	//Camera camera2;
	//
	//Framebuffer framebuffer;
	//Framebuffer framebufferTV;
	//GBuffer gbuffer;
	//
	//Mesh meshSphere;
	//Mesh meshSkybox;
	//Mesh meshLight;
	//Mesh meshPlane;
	//Mesh meshCube;
	//Mesh meshStan;
	//
	//GameObject goStan;
	//GameObject goSun;
	//GameObject goEarth;
	//GameObject goEarthPlane;
	//GameObject goMoon;
	//GameObject goJupiter;
	//GameObject goJupiterMoon[2];
	//GameObject goSaturn;
	//GameObject goSaturnRings;
	//GameObject goSkybox;
	//GameObject goTV;
	//
	//GameObject goLight;
	//
	//std::vector<GameObject*> goPlanets;
	//
	//// OpenGL Handles
	//ShaderProgram shaderBasic;
	////ShaderProgram shaderTexture;
	//ShaderProgram shaderSky;
	//ShaderProgram shaderTextureAlphaDiscard;
	//ShaderProgram shaderTextureJupiter;
	//
	//ShaderProgram shaderGbuffer;
	//ShaderProgram shaderPointLight;
	//
	//ShaderProgram shaderPassthrough;
	//ShaderProgram shaderGrayscale;
	//
	//UniformBuffer uniformBufferTime;
	//UniformBuffer uniformBufferLightScene;
	//UniformBuffer uniformBufferLight;
	//Light light;
	//UniformBuffer uniformBufferToon;
	//std::vector<Texture*> textureToonRamp;

	bool guiEnabled = true;

	//struct KeyInput
	//{
	//	bool moveLeft = false;
	//	bool moveRight = false;
	//	bool moveUp = false;
	//	bool moveDown = false;
	//	bool moveForward = false;
	//	bool moveBackward = false;
	//
	//	bool rotateUp = false;
	//	bool rotateDown = false;
	//	bool rotateLeft = false;
	//	bool rotateRight = false;
	//
	//
	//	bool ctrlL = false;
	//	bool ctrlR = false;
	//	bool shiftL = false;
	//	bool shiftR = false;
	//	bool altL = false;
	//	bool altR = false;
	//
	//	vec2 mouseMovement = vec2(0.0f);
	//	vec2 mousePos = vec2(0.0f);
	//	vec2 mousePosOld = vec2(0.0f);
	//	vec2 mousePosGUI = vec2(10.0f, 20.0f);
	//	
	//	bool f11 = false;
	//} input;
};
