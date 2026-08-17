#include "AudioManager.h"

void Audio::initializeAudio() {
	result = FMOD::System_Create(&system);
	result = system->init(512, FMOD_INIT_NORMAL, extradriverdata);
}

void Audio::playSound1()
{
	result = system->playSound(sound1, 0, false, &channel);
}

void Audio::playSound2()
{
	result = system->playSound(sound2, 0, false, &channel);
}

void Audio::loadSounds()
{
	result = system->createSound("sound1.wav", FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_OFF);

	result = system->createSound("sound2.wav", FMOD_DEFAULT, 0, &sound2);
	result = sound2->setMode(FMOD_LOOP_OFF);
}

void Audio::updateSound()
{
	result = system->update();
}