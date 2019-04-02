/*==============================================================================
3D Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2018.
This example shows how to basic 3D positioning of sounds.

Brent Cowan Jan. 28, 2019
==============================================================================*/
#pragma once
#pragma comment(lib, "FMOD/lib/fmod_vc.lib")

#include "FMOD/inc/fmod.hpp"
#include "FMOD/inc/fmod_errors.h"
#include <vector>

void FmodErrorCheck(FMOD_RESULT result);

struct Listener
{
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

	bool listening = false;
};

class SoundEngine
{
private:
	FMOD_RESULT result;
	unsigned int version;
	void *extradriverdata = 0;
	bool init = false;

public:
	~SoundEngine();
	void Init();
	void Update();

	FMOD::System *system;
	std::vector<Listener> listener;
};

bool LoadRawData(const char * file_name, FMOD::System* system, std::vector<signed short>* rawData, int* channels);

bool CreateSoundFromData(FMOD::System* system, FMOD::Sound **sound, int channels, unsigned int frequency, bool loop, std::vector<signed short>* rawData);

class Sound
{
private:
	static FMOD_RESULT result;
	FMOD::Channel *channel = 0;
	std::vector<signed short> rawData;
	bool is3D = false;
	bool loop = false;

	std::string name;

public:
	~Sound();
	bool Load(const char* fileName, bool _is3D = false, bool _loop = false);
	bool LoadRawData(const char * file_name, bool _is3D = false, bool _loop = false);
	bool CreateSoundFromData();
	int GetSampleSize();
	float GetSample(int location);
	void SetSample(int location, float value);
	FMOD::Channel* Play(FMOD_VECTOR startPos = { 0.0f, 0.0f, 0.0f }, FMOD_VECTOR startVel = { 0.0f, 0.0f, 0.0f });
	static void Stop(FMOD::Channel* thisChannel);
	static void SetPosition(FMOD::Channel* thisChannel, FMOD_VECTOR newPos, FMOD_VECTOR newVel = { 0.0f, 0.0f, 0.0f });

	static void setMode(FMOD::Channel* thisChannel, FMOD_MODE mode);
	static void linearFallOff(FMOD::Channel* thisChannel, bool isLinear);
	static void setMinAndMax(FMOD::Channel* thisChannel, float min, float max);

	FMOD::Sound *soundData = 0;
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

	static SoundEngine engine;

	void setName(std::string _NAME) { name = _NAME; };
	std::string getName() { return name; };
};