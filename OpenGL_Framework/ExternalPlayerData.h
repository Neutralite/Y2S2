#pragma once
#include "ResourceManager.h"

class ExternalPlayerData
{
public:
	static void initEPD();
	static void flushEPD() { initEPD(); };

	static std::vector<float> playerScores;
	static std::vector<int> playerNumbers;
	static std::vector<int> playerVehicleChoice;
	static std::vector<bool> playerActive;
	static std::vector<std::string> playerNames;
	static std::vector<vec3> playerColors;

	static void bootScreens();
	static std::vector<vec4> screenRats;
	static vec2 screenDims;
	static int numActive;
};


typedef ExternalPlayerData EPD;