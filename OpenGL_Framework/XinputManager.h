#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cmath>

#pragma comment(lib,"Xinput.lib")

//Buttons used for the controllers
enum CONTROLLER_INPUT_BUTTONS
{
	DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
	DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	THUMB_LEFT = XINPUT_GAMEPAD_LEFT_THUMB,
	THUMB_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB,
	SELECT = XINPUT_GAMEPAD_BACK,
	START = XINPUT_GAMEPAD_START
};

enum STICK_NAMES
{
	LS,
	RS
};

enum TRIGGER_NAMES
{
	LT,
	RT
};

//The X & Y input of the controller given as a float value from -1 -> 1 
struct Stick
{
	float x, y;
};

//The left and right trigger values that are  given as a float value from 0 -> 1
struct Triggers
{
	float LT, RT;
};

//typedef struct _XINPUT_VIBRATION {
//	WORD wLeftMotorSpeed;
//	WORD wRightMotorSpeed;
//} XINPUT_VIBRATION, *PXINPUT_VIBRATION;

class XinputController
{
public:
	//...
	void setControllerIndex(int _index)
	{
		this->index = _index;
	}

	void update()
	{
		if (4 > index && index >= 0)
		{
			XInputGetState(index, &info);
		}
	}

	void getSticks(Stick sticks[2])
	{
		float
		x = (float)info.Gamepad.sThumbLX / 32768,
		y = (float)info.Gamepad.sThumbLY / 32768;

		if (sqrt(x * x + y * y) > deadZoneStick)
		{


			if (info.Gamepad.sThumbLX < 0)
				sticks[0].x = (float)info.Gamepad.sThumbLX / 32768;
			else
				sticks[0].x = (float)info.Gamepad.sThumbLX / 32727;
			if (info.Gamepad.sThumbLY < 0)
				sticks[0].y = (float)info.Gamepad.sThumbLY / 32768;
			else
				sticks[0].y = (float)info.Gamepad.sThumbLY / 32727;
		}

		x = (float)info.Gamepad.sThumbRX / 32768;
		y = (float)info.Gamepad.sThumbRY / 32768;

		if(sqrt(x * x + y * y) > deadZoneStick)
		{
			if (info.Gamepad.sThumbRX < 0)
				sticks[1].x = (float)info.Gamepad.sThumbRX / 32768;
			else
				sticks[1].x = (float)info.Gamepad.sThumbRX / 32727;
			if (info.Gamepad.sThumbRY < 0)
				sticks[1].y = (float)info.Gamepad.sThumbRY / 32768;
			else
				sticks[1].y = (float)info.Gamepad.sThumbRY / 32727;
		}
	}

	void getSticks(Stick* s1, Stick* s2)
	{
		float
			x = (float)info.Gamepad.sThumbLX / 32768,
			y = (float)info.Gamepad.sThumbLY / 32768;

		if (sqrt(x * x + y * y) > deadZoneStick)
		{


			if (info.Gamepad.sThumbLX < 0)
				s1->x = (float)info.Gamepad.sThumbLX / 32768;
			else
				s1->x = (float)info.Gamepad.sThumbLX / 32727;
			if (info.Gamepad.sThumbLY < 0)
				s1->y = (float)info.Gamepad.sThumbLY / 32768;
			else
				s1->y = (float)info.Gamepad.sThumbLY / 32727;
		}

		x = (float)info.Gamepad.sThumbRX / 32768;
		y = (float)info.Gamepad.sThumbRY / 32768;

		if (sqrt(x * x + y * y) > deadZoneStick)
		{
			if (info.Gamepad.sThumbRX < 0)
				s2->x = (float)info.Gamepad.sThumbRX / 32768;
			else
				s2->x = (float)info.Gamepad.sThumbRX / 32727;
			if (info.Gamepad.sThumbRY < 0)
				s2->y = (float)info.Gamepad.sThumbRY / 32768;
			else
				s2->y = (float)info.Gamepad.sThumbRY / 32727;
		}
	}

	void deadZoneSticks(float dz)
	{
		deadZoneStick = dz;
	}

	void deadZoneTriggers(float dz)
	{
		deadZoneTrigger = dz;
	}

	bool isButtonPressed(int button)
	{
		return button & info.Gamepad.wButtons;
	}

	bool isButtonReleased(int button)
	{
		return !isButtonPressed(button);
	}

	void getTriggers(Triggers *triggers)
	{
		float l = (float)info.Gamepad.bLeftTrigger / 255, r = (float)info.Gamepad.bRightTrigger / 255;
		*triggers = Triggers{1 < deadZoneTrigger ? 0:l, r < deadZoneTrigger ? 0:r};
	}

	void setVibration(float lowRumble, float highRumble)
	{
		float _LR = lowRumble;
		float _HR = highRumble;

		if (_LR > 1.f)
			_LR = 1.f;
		if (_LR < 0.f)
			_LR = 0.f;
		if (_HR > 1.f)
			_HR = 1.f;
		if (_HR < 0.f)
			_HR = 0.f;

		vibrate.wLeftMotorSpeed = (WORD)(lowRumble * 65535.f);
		vibrate.wRightMotorSpeed = (WORD)(highRumble * 65535.f);
	}

	XINPUT_VIBRATION *getVibration()
	{
		return &vibrate;
	}

private:
	//...
	float deadZoneStick, deadZoneTrigger;
	XINPUT_STATE info;
	int index;
	XINPUT_VIBRATION vibrate = { 0, 0 };
};

class XinputManager
{
public:
	 //...
	XinputManager();

	static bool controllerConnected(int index);

	static XinputController* getController(int index);

	static void setRumble(int index, float lowRumble, float highRumble);

	static void update();
private:
	//...
	static XinputController controller[4];
};