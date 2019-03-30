#include "XinputManager.h"

XinputController XinputManager::controller[4];

XinputManager::XinputManager()
{
	for (int i = 0; i < 4; i++)
	{
		controller[i].setControllerIndex(i);
	}
}

bool XinputManager::controllerConnected(int index)
{
	if (index < 0 || index >= 4)
		return false;

	XINPUT_STATE connect;

	return XInputGetState(index, &connect) == ERROR_SUCCESS;
}

XinputController * XinputManager::getController(int index)
{
	if (index >= 0 && index < 4)
		return &controller[index];

	return nullptr;
}

void XinputManager::setRumble(int index, float lowRumble, float highRumble)
{
	if (controllerConnected(index))
	{
		XInputSetState(index, controller[index].getVibration());
	}
}

void XinputManager::update()
{
	for (int i = 0; i < 4; i++)
	{
		if (controllerConnected(i))
			controller[i].update();
	}
}
