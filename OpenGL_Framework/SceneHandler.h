#pragma once
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Scene.h"
#include <vector>

class SceneHandler
{
public:
	static bool initHandler(int windowWidth, int windowHeight);

	static void addScene(Scene* _S);
	static void destroyScene(Scene* _S);
	static void destroyScene(std::string scene_name);
	static void runScene(Scene* _S);
	static void endScene(Scene* _S);
	static void pauseScene(Scene* _S);
	static void resumeScene(Scene* _S);
	static void replaceScene(Scene* _S);
	static Scene* getCurrentScene();

	static void update();
	static void draw();
	static void GUI();

	static void keyboardDown(unsigned char key, int mouseX, int mouseY);
	static void keyboardUp(unsigned char key, int mouseX, int mouseY);
	static void keyboardSpecialDown(int key, int mouseX, int mouseY);
	static void keyboardSpecialUp(int key, int mouseX, int mouseY);
	static void mouseClicked(int button, int state, int x, int y);
	static void mouseMoved(int x, int y);
	static void mousePassive(int x, int y);
	static void reshapeWindow(int w, int h);

private:
	static Scene* searchScenes(Scene* _S);
	static Scene* searchScenes(std::string _NAME);
	static Scene* binarySearchScenes(Scene* _S, unsigned int front, unsigned int back);
	static Scene* binarySearchScenes(std::string _NAME, unsigned int front, unsigned int back);

	static int getLocationOfScene(Scene* _S);
	static int getLocationOfScene(std::string _NAME);
	static int getBinarySortLocationOfScene(Scene* _S, unsigned int front, unsigned int back);
	static int getBinarySortLocationOfScene(std::string _NAME, unsigned int front, unsigned int back);

	static void protectedAddScene(Scene* _S);
	static void protectedBinarySearchAndAddScene(Scene* _S, unsigned int front, unsigned int back);

	static void loadAllTextures(std::string &fileName);
	static void loadAllMeshes(std::string &fileName);
	static void loadAllObjects(std::string &fileName);
	static void loadAllUIElements(std::string &fileName);
	static void loadAllLights(std::string &filename);
	static void loadAllHitboxes(std::string &fileName);
	static void loadAllPlayerObjects(std::string &fileName);
	static void loadAllMorphTargs(std::string &fileName);
	static void loadAllShaders(std::string &fileName);
	static void loadAllFramebuffers(std::string &fileName);
	static void loadAllCameras(std::string &fileName);
	static void loadAllMaterials(std::string &fileName);
	static void loadAllFonts(std::string &fileName);
	 
	static void createChild(std::string &fileName, Transform* parent);

	static std::vector<Scene*> scenes;
	static Scene* currentScene;
};