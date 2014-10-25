#ifndef AUDIOTASK_H
#define AUDIOTASK_H

#include <string>
using std::string;

class AudioTask {
	public:
		AudioTask(const string& filename);
		~AudioTask();
		
		string filename;
};

#endif
