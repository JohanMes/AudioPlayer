#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(HWND hwnd) {
	this->hwnd = hwnd;
	this->device = "AudioPlayer";
	this->currenttask = 0;
	this->playing = false;
	this->opened = false;
}

AudioPlayer::~AudioPlayer() {
	for(unsigned int i = 0;i < playlist.size();i++) {
		delete playlist[i];
	}
}

bool AudioPlayer::SendString(const char* value) {
	DWORD result = mciSendString(value,NULL,0,hwnd); // returns zero on success...
	if(!result) {
		return true;
	} else {
		if(mciGetErrorString(result,errorbuffer,BUFFERSIZE)) {
			char caption[256];
			snprintf(caption,256,"Error sending command \"%s\"",commandbuffer);
			MessageBox(hwnd,errorbuffer,caption,MB_OK);
		}
		return false;
	}
}
void AudioPlayer::AddToPlaylist(const string& filename) {
	playlist.push_back(new AudioTask(filename));
}
bool AudioPlayer::Open() {
	if(playlist.size() == 0) {
		return false; // no files to open
	}
	
	snprintf(commandbuffer,BUFFERSIZE,"open \"%s\" alias %s",playlist[currenttask]->filename.c_str(),device.c_str());
	if(SendString(commandbuffer)) { // succesful
		opened = true;
		playing = false;
		return true;
	} else {
		return false; // file does not exist or PC refuses to cooperate?
	}
}
bool AudioPlayer::Close() {
	if(!opened) {
		return true; // no files to close
	}
	
	snprintf(commandbuffer,BUFFERSIZE,"close %s",device.c_str());
	if(SendString(commandbuffer)) { // succesful
		opened = false;
		playing = false;
		return true;
	} else {
		return false; // PC refuses to cooperate?
	}
}
bool AudioPlayer::Play() {
	if(playing) {
		return true; // already playing, good result
	}
	
	if(!opened) { // Our device does not yet know what to play. Tell it to open the current playlist file
		if(!Open()) { 
			return false; // playlist is empty or the PC refuses to play sound files
		}
	}
	
	// Our device now knows what to play, so simply resume...
	snprintf(commandbuffer,BUFFERSIZE,"play %s",device.c_str());
	if(SendString(commandbuffer)) { // succesful
		playing = true;
		InvalidateRect(hwnd,NULL,true);
		return true;
	} else {
		return false;
	}
}
bool AudioPlayer::Pause() {
	if(!playing) {
		return true; // already stopped playing, good result
	}
	
	if(!opened) { // There's nothing to pause. Quit.
		return true;
	}
	
	snprintf(commandbuffer,BUFFERSIZE,"pause %s",device.c_str());
	if(SendString(commandbuffer)) {
		playing = false;
		InvalidateRect(hwnd,NULL,true);
		return true;
	} else {
		return false;
	}
}
void AudioPlayer::PlayPauseToggle() {
	if(playing) {
		Pause();
	} else {
		Play();
	}
}
void AudioPlayer::Prev() {
	Close(); // close current
	if(currenttask == 0) { // wrap around
		currenttask = playlist.size()-1;
	} else {
		currenttask = (currenttask-1) % playlist.size(); // goto prev
	}
	Open();
	Play();
}
void AudioPlayer::Next() {
	Close(); // close current
	if(currenttask == (int)playlist.size()-1) { // wrap around
		currenttask = 0;
	} else {
		currenttask = (currenttask+1) % playlist.size(); // goto next
	}
	Open();
	Play();
}
bool AudioPlayer::IsPlaying() {
	return (opened && playing);
}
