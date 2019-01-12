#define _CRT_SECURE_NO_WARNINGS //Shut up microsoft

#include <GL\glew.h>
#include <windows.h>
#include <iostream>
#include "Game.h"


const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

Game *theGame;

/* function DisplayCallbackFunction(void)
 * Description:
 *  - this is the openGL display routine
 *  - this draws the sprites appropriately
 */
void DisplayCallbackFunction(void)
{
	theGame->draw();
}

/* function void KeyboardCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is pressed
 */

void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardDown(key, x, y);

}
/* function void KeyboardUpCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is lifted
 */
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardUp(key, x, y);
}

/* function TimerCallbackFunction(int value)
 * Description: 
 *  - this is called many times per second
 *  - this enables you to animate things
 *  - no drawing, just changing the state
 *  - changes the frame number and calls for a redisplay
 *  - FRAME_DELAY_SPRITE is the number of milliseconds to wait before calling the timer again
 */
void TimerCallbackFunction(int value)
{
	theGame->update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY_SPRITE, TimerCallbackFunction, 0);
}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	theGame->mouseClicked(button,state, x, y);
	glutPostRedisplay();
}

/* function MouseMotionCallbackFunction()
 * Description:
 *   - this is called when the mouse is clicked and moves
 */
void MouseMotionCallbackFunction(int x, int y)
{
	theGame->mouseMoved(x, y);
	glutPostRedisplay();
}

void MousePassiveCallbackFunction(int x, int y)
{
	theGame->mouseMoved(x, y);
	glutPostRedisplay();
}

void NewWindowShape(int W, int H)
{
	//theGame->resizeCameras((float)W / (float)H);
	glViewport(0, 0, W, H);
}

void CALLBACK OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
{
	std::cout << "CALLBACK\n";
	char buffer[9] = { '\0' };
	sprintf(buffer, "%.8x", id);

	std::string message("OpenGL(0x");
	message += buffer;
	message += "): ";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		message += "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		message += "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		message += "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		message += "Portability Issue";
		break;
	case GL_DEBUG_TYPE_MARKER:
		message += "Stream Annotation";
		break;
	case GL_DEBUG_TYPE_OTHER_ARB:
	default:
		message += "OTHER";
		break;
	}
	message += "\nSauce: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		message += "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		message += "Widnow System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		message += "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		message += "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		message += "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
	default:
		message += "Other";
		break;
	}
	message += "\nSeverity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		message += "SNOOP DOG";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		message += "MEDIOCRE";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		message += "NOT EVEN";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		message += "NOTIFICATION";
		break;
	}

	message += "\n";
	message += msg;
	message += "\n";

	if (type == GL_DEBUG_TYPE_ERROR)
		std::cout << "ERROR: " << message << std::endl;
	else
		std::cout << "WAT? " << message << std::endl;
}

void InitOpenGLDebugCallback()
{
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OpenGLDebugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("HI");

	glewExperimental = true;


	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW COULD NOT BE INITIALIZED\n.";
		system("pause");
		return 0;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutPassiveMotionFunc(MousePassiveCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	glutReshapeFunc(NewWindowShape);
	
#ifdef _DEBUG
	InitOpenGLDebugCallback();
#endif

	/* init the game */
	theGame = new Game();
	theGame->initializeGame();

	/* start the game */
	glutMainLoop();
	return 0;
}