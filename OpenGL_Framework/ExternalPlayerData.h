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
};


typedef ExternalPlayerData EPD;