#pragma once
#include <windows.h>

#include "Shader.h"
#include "MESH.h"
#include "Texture.h"
#include "FrameBuffer.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"
#include "Camera.h"
#include "Transform.h"
#include "Object.h"
#include <vector>
#include "Field.h"
#include <fstream>

#include "Jmath.h"
#include "clickBox.h"
#include "LIGHT_SOURCE.h"
#include "Player.h"
#include "Destructable.h"

#define WINDOW_WIDTH			800 //glutGet(GLUT_WINDOW_WIDTH)
#define WINDOW_HEIGHT			432 //glutGet(GLUT_WINDOW_HEIGHT)
#define FRAMES_PER_SECOND		60
#define BLOOM_THRESHOLD			0.25f
#define BLOOM_DOWNSCALE			2.0f
#define BLOOM_BLUR_PASSES		4

class Game
{
public:
	Game();
	~Game();

	virtual void initializeGame();
	virtual void update();
	virtual void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	void setKeysDown(bool down, unsigned char key);
	void keyHandler();
	void mouseHandler();
	void triggerHandler();
	void setCursorVisible(bool b) { if (!b)glutSetCursor(GLUT_CURSOR_NONE); else glutSetCursor(GLUT_CURSOR_LEFT_ARROW); };

	void addPostProcessLink(FrameBuffer* FB, std::vector<FrameBuffer*> PrevTexArray, std::vector<bool> allowEffect);
	void addPostProcessLink(FrameBuffer* FB, FrameBuffer* PrevTexArray, bool allowEffect);

	//void addAllPostProcessLink(FrameBuffer* FB, std::vector<FrameBuffer*> PrevTexArray, std::vector<bool> allowEffect);
	//void addAllPostProcessLink(FrameBuffer* FB, FrameBuffer* PrevTexArray, bool allowEffect);

	bool dealWithCol(Object* O1, Object* O2);

	bool dealWithExplosions(Object* EXP, Object* DESTR);

	bool dealWithDestruction(Object* defender, Object* offender);

	void drawMines(Object* _OBJ, mat4 previous, ShaderProgram *SP);
	void drawExplosions(Object* _OBJ, mat4 previous, ShaderProgram *SP);

	void drawObjectMesh(Object* _OBJ, mat4 previous, ShaderProgram *SP, vec3 B1P, vec3 B2P, float tT);
	void drawUIObject(Object* _OBJ, mat4 previous, ShaderProgram *SP);
	void drawOnlyPlayer(Object* _OBJ, mat4 previous, ShaderProgram *SP);

	void loadAllTextures(std::string &fileName);
	void loadAllMeshes(std::string &fileName);
	void loadAllObjects(std::string &fileName);
	void loadAllUIElements(std::string &fileName);
	void loadAllLights(std::string &filename);
	void loadAllHitboxes(std::string &fileName);
	void loadAllPlayerObjects(std::string &fileName);
	void loadAllMorphTargs(std::string &fileName);

	void createChild(std::string &fileName, Object* parent, Transform TF);

	void resetMap();

	void reshapeWindow(int w, int h);

	//void print(int x, int y, int z, char *string);
	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;
	float DT = 0.017f;

	//void resizeCameras(float ASPect);

private:
	bool keysDown[256];
	bool backCheckKeysDown[256];
	bool mouseDown[10];
	bool backCheckMouseDown[10];

	std::vector<std::string> Triggers;
	std::vector<Object*> parents;
	std::vector<Object*> objects;
	std::vector<Object*> toDraw;
	std::vector<Object*> UIELEM;
	std::vector<Object*> staticCollisions;
	std::vector<Object*> dynamicCollisions;
	std::vector<Object*> toUpdate;

	std::vector<Object*> players;
	std::vector<Object*> SHAKEYOBJ;
	std::vector<Object*> SHAKEYBUFFER;

	std::vector<Object*> MINEZ;
	float MAX_MINE_TIMER = 1.f;
	std::vector<float> MINETIMER;
	std::vector<Object*> EXPLOSIONS;
	float MAX_EXP_TIMER = 1.f;
	std::vector<float> SPLODETIMER;
	std::vector<Object*> GONE;
	float MAX_GONE_TIMER = 1.f;
	std::vector<float> GONETIMER;

	Object* BASE_PLATE_SAND;
	Object* BASE_PLATE_DIRT;
	Object* BASE_PLATE_CONCRETE;
	Object* BASE_PLATE_GRASS;
	Object* BASE_PLATE_ROAD;
	Object* BASE_PLATE_4WAY;
	Object* BASE_PLATE_T;
	Object* BASE_PLATE_CORNER;

	Object* HEALTH_BAR;
	Object* POINTS_BAR;
	Object* TIME_BAR;
	Object* SPED_BAR;

	Object* MINE_OBJ;
	Object* EXP_OBJ;

	Light* SUN;
	std::vector<mat4> lightMats;
	std::vector<Light*> lightsToDraw;

	std::vector<Texture*> baseTextures;
	std::vector<TextureLayer*> textures;
	std::vector<Mesh*> meshList;
	std::vector<FrameBuffer*> postProcEffects;
	std::vector<Light*> allLight; //PLUS ULTRA
	std::vector<Hitbox2D*> allHitboxes;
	//std::vector<Camera*> allCams;
	
	vec2 mousePosition, prevMousePosition;
	vec2 DeltaMousePos;
	float ambientLevel = 0.3f;

	std::vector<vec4> placeAndTime;
	unsigned int currentBulge = 0;
	float currentCoolDown;
	float coolDownExp = 0.1f;

	vec3 currentGoal;
	int pcX, pcY;
	vec3 spawnPoint;

	float aspectRatio = 800.f / 432.f;
	float tileSize = 6.f;
	float cameraHeight = 12.f;

	ShaderProgram PassThrough;
	//ShaderProgram PLAYA;
	ShaderProgram GreyScalePost;
	ShaderProgram BloomHP;
	ShaderProgram BloomVB;
	ShaderProgram BloomHB;
	ShaderProgram BloomCOMP;

	ShaderProgram UI_SHADER;
	ShaderProgram EXPLOSIONSHADER;
	ShaderProgram MINESHADER;

	ShaderProgram COMIC_SETUP;
	ShaderProgram COMIC_EXECUTION;
	Texture* overlay;

	Camera camera;
	Camera UIcam;

	//FrameBuffer FB;
	//FrameBuffer testBuff;
	//FrameBuffer WorkBuffer1;
	//FrameBuffer WorkBuffer2;
	//FrameBuffer UISCREEN;

	FrameBuffer CaptureScene;
	//FrameBuffer ComicScene;

	Field *theMap;
	Object* TOP_WALL;
	Object* BOTTOM_WALL;
	Object* LEFT_WALL;
	Object* RIGHT_WALL;

	//clickBox SAVE_BUTTON;

	//bool bloomActive = false;
	//bool normalRenderActive = true;
	//bool rippleActive = false;
	//bool paused = false;
	bool swapGraphics = false;

	int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;
};
