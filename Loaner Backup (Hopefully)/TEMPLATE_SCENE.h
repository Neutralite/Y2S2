#pragma once
//DO NOT USE THIS AS AN ACTUAL SCENE. THIS IS MERELY SO YOU KNOW THE BARE MINIMUM A SCENE MUST CONTAIN.

#include "Game.h"

class TEMPLATE_SCENE : public Game
{
	void initializeGame();
	void update();
	void draw();

	//void keyboardDown(unsigned char key, int mouseX, int mouseY);
	//void keyboardUp(unsigned char key, int mouseX, int mouseY);
	//void mouseClicked(int button, int state, int x, int y);
	//void mouseMoved(int x, int y);

	//other functions:

	//void drawObjectMesh(Object* _OBJ, mat4 previous);
	//This takes in an object and draw its mesh and all of its children.
	//'previous' is simply the orientation of its parents, if they exist.
	//if you're passing in an object with no parents, simply pass the identity matrix into previous.

	//void setKeysDown(bool down, unsigned char key);
	//void keyHandler();
	//These two are what set keysDown and backCheckKeysDown respectively (both are arrays of bools).
	//keysDown determines if a key is down or up, whereas backCheckKeysDown determines if they were down on the previous frame
	
	//void setWarpMouseToCenter(bool b);
	//void setCursorVisible(bool b);
	//These two allow you to control whether or not the mouse is locked onto the center of the screen, and whether or not you can see the mouse respectively.

	//void triggerHandler();
	//Handles triggers for animation and particle system purposes. Updates them all for you.

	//
};