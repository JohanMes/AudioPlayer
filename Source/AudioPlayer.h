#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <windows.h>
#include "AudioTask.h"

// should be enough for any filename
#define BUFFERSIZE 8192

class AudioPlayer {
	vector<AudioTask*> playlist;
	int currenttask;
	string device;
	HWND hwnd;
	bool playing; // true if a file is loaded AND it's playing
	bool opened; // true if a file is loaded and playable/pausable
	char commandbuffer[BUFFERSIZE]; // please use the 'n' version of print commands
	char errorbuffer[BUFFERSIZE];
	
	bool SendString(const char* value); // macro for mciSendString, returns TRUE on success
	bool Open(); // prepares device by opening a file
	bool Close(); // ends current file, so the user can open a new one
	public:
		AudioPlayer(HWND hwnd);
		~AudioPlayer();
		
		void AddToPlaylist(const string& filename);
		
		bool IsPlaying();
		bool Play(); // returns false if playlist is empty
		bool Pause(); // returns false when pausing fails
		void Prev();
		void Next();
		void PlayPauseToggle();
};

#endif
