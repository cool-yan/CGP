#include "AudioManager.h"

void AudioManager::initializeAudio() {
	//create virtual sound card and initialize the channel
	result = FMOD::System_Create(&system);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

void AudioManager::playSound1()
{
	//play sound1 on the channel
	//channel is set to 0 so that the sound can be played on any available channel
	//the last parameter is a pointer to the channel that the sound is played on
	//if the sound is already playing on a channel, it will be stopped and played on the new channel
	//the scope of the channel variable is limited to the playSound1 function, so it will not be accessible outside of this function


	result = system->playSound(sound1, 0, false, &channel);
}

void AudioManager::playSound2()
{
	//play sound2 on the channel
	result = system->playSound(sound2, 0, true, &channel);
	channel->setPan(sound2pan);
	channel->setPaused(false);
}

void AudioManager::loadSounds()
{
	//load the sound files and set them to not loop
	result = system->createSound("assets/Audio/explosionCrunch_000.ogg", FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_OFF);

	//create stream will only load a small portion of the sound file into memory, and will stream the rest of the sound file from disk as needed
	// A stream only has one decode buffer and file handle, and therefore can only be played once. 
	// It cannot play multiple times at once because it cannot share a stream buffer if the stream is playing at different positions. 
	// Open multiple streams to have them play concurrently.

	result = system->createSound("assets/Audio/doorOpen_000.ogg", FMOD_DEFAULT, 0, &sound2);
	result = sound2->setMode(FMOD_LOOP_OFF);
}

void AudioManager::updateSound()
{
	result = system->update();
}