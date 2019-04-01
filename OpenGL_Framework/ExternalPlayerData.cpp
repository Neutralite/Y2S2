#include "ExternalPlayerData.h"

std::vector<float> EPD::playerScores;
std::vector<int> EPD::playerNumbers;
std::vector<int> EPD::playerVehicleChoice;
std::vector<bool> EPD::playerActive;
std::vector<std::string> EPD::playerNames;
std::vector<vec3> EPD::playerColors;
std::vector<vec4> EPD::screenRats;
vec2 EPD::screenDims;
int EPD::numActive;

void ExternalPlayerData::initEPD()
{
	playerScores.clear();
	playerNumbers.clear();
	playerActive.clear();
	playerNames.clear();
	playerVehicleChoice.clear();
	playerColors.clear();

	playerScores.resize(4);
	playerNumbers.resize(4);
	playerActive.resize(4);
	playerNames.resize(4);
	playerVehicleChoice.resize(4);
	playerColors.resize(4);

	for (int i = 0; i < 4; i++)
	{
		playerScores[i] = 0.f;
		playerNumbers[i] = -1;
		playerActive[i] = false;
		playerNames[i] = "_NULL";
		playerVehicleChoice[i] = -1;
	}

	playerColors[0] = vec3(0.2f, 0.2f, 1.f);
	playerColors[1] = vec3(1.f, 0.2f, 0.2f);
	playerColors[2] = vec3(0.2f, 1.f, 0.2f);
	playerColors[3] = vec3(1.f, 1.f, 0.2f);
}

void ExternalPlayerData::bootScreens()
{
	screenRats.clear();
	screenRats.resize(4, vec4());
	int totScreens = 0;

	for (int i = 0; i < 4; i++)
	{
		if (playerActive[i])
		{
			totScreens++;
		}
	}

	numActive = totScreens;

	if (totScreens == 1)
	{
		screenDims = vec2(1, 1);
		totScreens = 0;
		for (int i = 0; i < 4; i++)
		{
			if (playerActive[i])
			{
				screenRats[i] = vec4(0, 0, 
									1, 1);
				totScreens++;
			}
		}
	}
	else if (totScreens == 2)
	{
		screenDims = vec2(0.5f, 1);
		totScreens = 0;
		for (int i = 0; i < 4; i++)
		{
			if (playerActive[i])
			{
				screenRats[i] = vec4(0 + 0.5f * (float)totScreens, 0, 
									0.5f + 0.5f * (float)totScreens, 1);
				totScreens++;
			}
		}
	}
	else if (totScreens == 3)
	{
		screenDims = vec2(0.5f, 0.5f);
		totScreens = 0;
		for (int i = 0; i < 4; i++)
		{
			if (playerActive[i])
			{
				screenRats[i] = vec4(0 + 0.5f * (float)(totScreens % 2), 0 + 0.5f * (float)(((totScreens + 2) / 2) % 2),
					0.5f + 0.5f * (float)(totScreens % 2), 0.5f + 0.5f * (float)(((totScreens + 2) / 2) % 2)); 
				totScreens++;
			}
		}
	}
	else if (totScreens == 4)
	{
		screenDims = vec2(0.5f, 0.5f);
		totScreens = 0;
		for (int i = 0; i < 4; i++)
		{
			if (playerActive[i])
			{
				screenRats[i] = vec4(0 + 0.5f * (float)(totScreens % 2), 0 + 0.5f * (float)(((totScreens + 2) / 2) % 2),
					0.5f + 0.5f * (float)(totScreens % 2), 0.5f + 0.5f * (float)(((totScreens + 2) / 2) % 2));
				totScreens++;
			}
		}
	}
}
