#include "FmodWrapper.h"
#include <Windows.h>
#include <iostream>
using namespace std;

signed short* dataPtr = NULL;

FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND *sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
	//This is useful if the user calls Channel::setPosition and you want to seek your data accordingly.
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *sound, void *data, unsigned int datalen)
{
	unsigned int count;
	static long current = 0;
	signed short *stereo16bitbuffer = (signed short *)data;

	for (count = 0; count < datalen >> 1; count++)        // >>2 = 16bit stereo (4 bytes per sample)
	{
		*stereo16bitbuffer++ = dataPtr[current];    // left channel
	   // *stereo16bitbuffer++ = dataPtr[current+1];    // right channel

		current++;//+= 2;
	}

	return FMOD_OK;
}

void FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << FMOD_ErrorString(result) << endl;
		system("pause");
	}
}

SoundEngine::~SoundEngine()
{
	if (init)
	{
		//Free up resources
		result = system->close();
		FmodErrorCheck(result);
		result = system->release();
		FmodErrorCheck(result);
	}
}

void SoundEngine::Init()
{
	if (!init)
	{
		listener.resize(4);
		//Create a System object and initialize.
		result = FMOD::System_Create(&system);
		if (result != FMOD_OK) return;
		FmodErrorCheck(result);

		result = system->getVersion(&version);
		FmodErrorCheck(result);

		result = system->init(1000, FMOD_INIT_NORMAL, extradriverdata);
		FmodErrorCheck(result);

		//Set the distance units. (meters/feet etc).
		result = system->set3DSettings(1.0, 1.0, 1.0f);
		FmodErrorCheck(result);

		init = true;
	}
}

void SoundEngine::Update()
{
	int numListeners = 0;
	int activeListener = 0;
	for (int i = 0; i < 4; i++)
	{
		if (listener[i].listening)
			numListeners++;
	}

	system->set3DNumListeners(numListeners);

	for (int i = 0; i < 4; i++)
	{
		if (listener[i].listening)
		{
			result = system->set3DListenerAttributes(activeListener, &listener[i].pos, &listener[i].vel, &listener[i].forward, &listener[i].up);
			FmodErrorCheck(result);
			activeListener++;
		}
	}
	//std::cout << "UPDATING" << std::endl;
	result = system->update();
	FmodErrorCheck(result);
}

SoundEngine Sound::engine;
FMOD_RESULT Sound::result;

Sound::~Sound()
{
	if (soundData)
	{
		result = soundData->release();
		FmodErrorCheck(result);
	}
}

bool Sound::Load(const char* fileName, bool _is3D, bool _loop)
{
	engine.Init();

	this->is3D = _is3D;
	this->loop = _loop;
	FMOD_MODE mode = FMOD_DEFAULT;
	if (is3D) mode = mode | FMOD_3D;
	else mode = mode | FMOD_2D;

	if (loop) mode = mode | FMOD_LOOP_NORMAL;
	else mode = mode | FMOD_LOOP_OFF;

	result = engine.system->createSound(fileName, mode, 0, &soundData);
	if (result != FMOD_OK) return false;
	FmodErrorCheck(result);

	if (is3D)
	{
		result = soundData->set3DMinMaxDistance(1.0f, 500.0f);
		FmodErrorCheck(result);
	}

	return true;
}

FMOD::Channel* Sound::Play(FMOD_VECTOR startPos, FMOD_VECTOR startVel)
{
	result = engine.system->playSound(soundData, 0, true, &channel);
	FmodErrorCheck(result);

	if (is3D)
	{
		result = channel->set3DAttributes(&startPos, &startVel);
		FmodErrorCheck(result);
	}

	result = channel->setPaused(false);
	FmodErrorCheck(result);

	return channel;
}

void Sound::Stop(FMOD::Channel* thisChannel)
{
	if (thisChannel)
	{
		thisChannel->stop();
		thisChannel = NULL;
	}
}

void Sound::SetPosition(FMOD::Channel* thisChannel, FMOD_VECTOR newPos, FMOD_VECTOR newVel)
{
	//Check to make sure the channel is playing
	bool isPlaying;
	thisChannel->isPlaying(&isPlaying);
	if (isPlaying)
	{
		result = thisChannel->set3DAttributes(&newPos, &newVel);
		FmodErrorCheck(result);
	}
}


void Sound::setMode(FMOD::Channel * thisChannel, FMOD_MODE mode)
{
	thisChannel->setMode(mode);
	//std::cout << mode << std::endl;
}

void Sound::linearFallOff(FMOD::Channel * thisChannel, bool isLinear)
{
	if (isLinear)
		setMode(thisChannel, FMOD_3D_LINEARROLLOFF);
	else
		setMode(thisChannel, FMOD_3D_INVERSEROLLOFF);
}

void Sound::setMinAndMax(FMOD::Channel* thisChannel, float min, float max)
{
	bool playing;
	thisChannel->isPlaying(&playing);

	FMOD_MODE fmode;
	thisChannel->getMode(&fmode);

	if (playing && (fmode & FMOD_3D) == FMOD_3D)
	{
		result = thisChannel->set3DMinMaxDistance(min, max);
		FmodErrorCheck(result);
	}
}

bool Sound::LoadRawData(const char * file_name, bool _is3D, bool _loop)
{
	engine.Init();

	this->is3D = _is3D;
	this->loop = _loop;

	result = engine.system->createStream(file_name, FMOD_OPENONLY | FMOD_ACCURATETIME, 0, &soundData);
	FmodErrorCheck(result);

	void *data;
	unsigned int length = 0, read;

	//Get the number of samples
	result = soundData->getLength(&length, FMOD_TIMEUNIT_PCMBYTES);
	FmodErrorCheck(result);

	//Get the number of channels
	FMOD_SOUND_TYPE type;
	FMOD_SOUND_FORMAT format;
	int bits;
	int channels;
	soundData->getFormat(&type, &format, &channels, &bits);
	
	//Make sure it is a mono sound
	if (channels != 1)
	{
		cout << "Error: Stereo sounds cannot be edited" << endl;
		soundData->release();
		soundData = NULL;
		return false;
	}

	//Get the raw data
	rawData.clear();
	rawData.reserve(length / 2);
	signed short tmpData;
	data = (void*)&tmpData;

	//Copy the raw data
	for (unsigned int i = 0; i < length / 2; i++)
	{
		result = soundData->readData((char *)data, 2, &read);
		if (result == FMOD_OK && read == 2)
		{
			rawData.push_back(tmpData);
		}
		else
		{
			i = length / 2;
		}
	}

	soundData->release();
	soundData = NULL;
	return true; //It worked!
}

bool Sound::CreateSoundFromData()
{
	//If the raw data does not exist, return
	if (rawData.size() < 4) { return false; }

	FMOD_MODE mode = FMOD_DEFAULT | FMOD_OPENUSER;
	if (is3D) mode = mode | FMOD_3D;
	else mode = mode | FMOD_2D;

	if (loop) mode = mode | FMOD_LOOP_NORMAL;
	else mode = mode | FMOD_LOOP_OFF;

	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	dataPtr = &rawData[0];

	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	createsoundexinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
	createsoundexinfo.decodebuffersize = rawData.size();                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	createsoundexinfo.length = rawData.size() * 2;//44100 * channels * sizeof(signed short) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
	createsoundexinfo.numchannels = 1;                                    /* Number of channels in the sound. */
	createsoundexinfo.defaultfrequency = 44000;                                       /* Default playback rate of sound. */
	createsoundexinfo.format = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
	createsoundexinfo.pcmreadcallback = pcmreadcallback;                             /* User callback for reading. */
	createsoundexinfo.pcmsetposcallback = pcmsetposcallback;                           /* User callback for seeking. */

	result = engine.system->createSound(0, mode, &createsoundexinfo, &soundData);
	FmodErrorCheck(result);

	return true;
}

int Sound::GetSampleSize()
{
	return rawData.size();
}

float Sound::GetSample(int location)
{
	if(loop)
	{
		return float(rawData[location % rawData.size()])/ 32767.0f;
	}
	else
	{
		if (location < 0 || (unsigned int)location > rawData.size() - 1)
		{
			return 0.0f;
		}
		else
		{
			return float(rawData[location])/32767.0f;
		}
	}
	
}

void Sound::SetSample(int location, float value)
{
	static int tempInt;
	if (location > -1 && (unsigned int)location < rawData.size())
	{
		tempInt = int(value*32767.0f);
		if (tempInt >= (int)32767.0f) { tempInt =  (int)32767.0f; }
		if (tempInt < (int)-32767.0f) { tempInt = (int)-32767.0f; }
		rawData[location] = (signed short)tempInt;
	}
}

bool LoadRawData(const char * file_name, FMOD::System* system, std::vector<signed short>* rawData, int* channels)
{
	FMOD::Sound* sound;

	FMOD_RESULT result;
	result = system->createStream(file_name, FMOD_OPENONLY | FMOD_ACCURATETIME, 0, &sound);
	FmodErrorCheck(result);

	void *data;
	unsigned int length = 0, read;

	//Get the number of samples
	result = sound->getLength(&length, FMOD_TIMEUNIT_PCMBYTES);
	FmodErrorCheck(result);

	//Get the number of channels
	FMOD_SOUND_TYPE type;
	FMOD_SOUND_FORMAT format;
	int bits;
	sound->getFormat(&type, &format, channels, &bits);

	//Get the raw data
	rawData->clear();
	rawData->reserve(length / 2);
	signed short tmpData;
	data = (void*)&tmpData;

	for (unsigned int i = 0; i < length / 2; i++)
	{
		result = sound->readData((char *)data, 2, &read);
		if (result == FMOD_OK && read == 2)
		{
			rawData->push_back(tmpData);
		}
		else
		{
			i = length / 2;
		}
	}

	return true;
}



bool CreateSoundFromData(FMOD::System* system, FMOD::Sound **sound, int channels, unsigned int frequency, bool loop, std::vector<signed short>* rawData)
{
	FMOD_RESULT   result;
	FMOD_MODE mode = FMOD_2D | FMOD_OPENUSER;
	if (loop) { mode = mode | FMOD_LOOP_NORMAL; }
	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	dataPtr = &((*rawData)[0]);

	if (channels < 1) { channels = 1; }
	if (channels > 2) { channels = 2; }

	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	createsoundexinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
	createsoundexinfo.decodebuffersize = rawData->size();                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	createsoundexinfo.length = rawData->size() * 2;//44100 * channels * sizeof(signed short) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
	createsoundexinfo.numchannels = channels;                                    /* Number of channels in the sound. */
	createsoundexinfo.defaultfrequency = frequency;                                       /* Default playback rate of sound. */
	createsoundexinfo.format = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
	createsoundexinfo.pcmreadcallback = pcmreadcallback;                             /* User callback for reading. */
	createsoundexinfo.pcmsetposcallback = pcmsetposcallback;                           /* User callback for seeking. */

	result = system->createSound(0, mode, &createsoundexinfo, sound);
	FmodErrorCheck(result);

	return true;
}