#include "ExternalPlayerData.h"

std::vector<float> EPD::playerScores;
std::vector<int> EPD::playerNumbers;
std::vector<int> EPD::playerVehicleChoice;
std::vector<bool> EPD::playerActive;
std::vector<std::string> EPD::playerNames;

void ExternalPlayerData::initEPD()
{
	playerScores.clear();
	playerNumbers.clear();
	playerActive.clear();
	playerNames.clear();
	playerVehicleChoice.clear();

	playerScores.resize(4);
	playerNumbers.resize(4);
	playerActive.resize(4);
	playerNames.resize(4);
	playerVehicleChoice.resize(4);

	for (int i = 0; i < 4; i++)
	{
		playerScores[i] = 0.f;
		playerNumbers[i] = -1;
		playerActive[i] = false;
		playerNames[i] = "_NULL";
		playerVehicleChoice[i] = -1;
	}
}
