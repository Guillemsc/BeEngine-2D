#ifndef __MODULE_AUDIO_H__
#define __MODULE_AUDIO_H__

#include "Module.h"

#include <fmod.hpp>
#include <fmod_errors.h>

class AudioSound
{
	friend class ModuleAudio;

public:
	void Play();
	void Stop();
	void SetPaused(bool set);

	bool GetPlaying();
	bool GetPaused();

private:
	std::string path;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* chanel = nullptr;
};

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	AudioSound* CreateSound(const std::string path);

	void PlayAudioSound(AudioSound* sound);
	void StopAudioSound(AudioSound* sound);
	void SetPausedAudioSound(AudioSound* sound, bool paused);

private:
	FMOD::System* fmod_system = nullptr;

	std::vector<AudioSound*> sounds;

	AudioSound* sound = nullptr;
};

#endif // !__MODULE_AUDIO_H__