#pragma once
#include "fmod.hpp"

class Audio
{
public:
	FMOD::System* system;
	FMOD::Sound *sound1, *sound2;
	FMOD::Channel *channel = 0;
	FMOD_RESULT result;
	void *extradriverdata = 0;

	void initializeAudio();
	void playSound1();
	void playSound2();
	void loadSounds();
	void updateSound();

	Audio();
	~Audio();
};