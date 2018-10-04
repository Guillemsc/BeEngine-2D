#ifndef __MODULE_AUDIO_H__
#define __MODULE_AUDIO_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Awake();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

private:
	Mix_Music*			music = nullptr;
	std::list<Mix_Chunk*>	fx;
};

#endif // __MODULE_AUDIO_H__