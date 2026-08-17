#pragma once
#include <iostream>
#include "fmod.hpp"

class AudioManager
{
public:
	FMOD::System* system;			//virtual sound card
	FMOD::Sound *sound1, *sound2, *tempSound;	//sound files
	float sound1pan = 0.0f, sound2pan = 0.0f, soundTempPan = 0.0f;	//sound pan values
	FMOD::Channel* channel = 0;		//channel to play sound
	FMOD_RESULT result;				//result of FMOD operations
	void* extradriverdata = 0;		//extra driver data

	void initializeAudio();
	void playSound1();
	void playSound2();
	void loadSounds();
	void updateSound();
	void playSound(const char* soundFilePath, bool isStream = false) {
		
		result = system->createSound(soundFilePath, isStream ? FMOD_CREATESTREAM : FMOD_DEFAULT, 0, &tempSound);
		if (result) {
			std::cout << "Failed to load sound: " << soundFilePath;
		}
		result = tempSound->setMode(FMOD_LOOP_OFF);

		system->playSound(tempSound, 0, false, &channel);

	}

	AudioManager() {};
	~AudioManager() {};
};