#include "Globals.h"
#include "App.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

#pragma comment( lib, "FMod/Lib/fmod_vc.lib")
#pragma comment( lib, "FMod/Lib/fmodL_vc.lib")

ModuleAudio::ModuleAudio() : Module()
{
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Loading Audio Mixer");

	if (FMOD::System_Create(&fmod_system) != FMOD_OK)
	{
		INTERNAL_LOG("Error creating FMod system");

		ret = false;
	}
	else
	{
		int driverCount = 0;
		fmod_system->getNumDrivers(&driverCount);

		if (driverCount == 0)
		{
			INTERNAL_LOG("Error creating FMod system, no drivers found!");

			ret = false;
		}
		else
		{
			fmod_system->init(36, FMOD_INIT_NORMAL, NULL);
		}
	}

	return ret;
}

bool ModuleAudio::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleAudio::Update()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	return ret;
}

AudioSound* ModuleAudio::CreateSound(const std::string path)
{
	AudioSound* ret = nullptr;

	FMOD::Sound* sound = nullptr;
	if (fmod_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK)
	{
		ret = new AudioSound();

		ret->path = path;
		ret->sound = sound;

		ret->sound->setMode(FMOD_LOOP_OFF);

		sounds.push_back(ret);
	}

	return ret;
}

void ModuleAudio::PlayAudioSound(AudioSound * sound)
{
	if (sound != nullptr)
	{
		fmod_system->playSound(sound->sound, 0, false, &sound->chanel);
	}
}

void ModuleAudio::StopAudioSound(AudioSound * sound)
{
	if (sound != nullptr)
	{
		if (sound->chanel != nullptr)
		{
			sound->chanel->stop();

			sound->chanel = nullptr;
		}
	}
}

void ModuleAudio::SetPausedAudioSound(AudioSound * sound, bool paused)
{
	if (sound != nullptr)
	{
		if (sound->chanel != nullptr)
		{
			sound->chanel->setPaused(paused);
		}
	}
}

void AudioSound::Play()
{
	App->audio->PlayAudioSound(this);
}

void AudioSound::Stop()
{
	App->audio->StopAudioSound(this);
}

void AudioSound::SetPaused(bool set)
{
	App->audio->SetPausedAudioSound(this, set);
}

bool AudioSound::GetPlaying()
{
	bool ret = false;

	if (chanel != nullptr)
	{
		chanel->isPlaying(&ret);
	}

	return ret;
}

bool AudioSound::GetPaused()
{
	bool ret = false;

	if (chanel != nullptr)
	{
		chanel->getPaused(&ret);
	}

	return ret;
}
