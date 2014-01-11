#include "includes.h"
#include "LMRecorder.h"
#include "LeapProcess.h"

using namespace std;

#ifndef FILELISTENER_H_
#define FILELISTENER_H_

class FileListener {

public:

	LMRecorder lr;
	LeapProcess *process;
	GestureFrame *frame;
	pthread_t frameLoaderThread;

	FileListener();
	void attachToProcess(LeapProcess *lp);
	void readFile(string filePath);

private:
	vector<GestureFrame> frames;
	void convert(Frame inputFrame);
	static void *runLoader(void *c);
	void *frameLoader(void);

};

#endif /* FILELISTENER_H_ */
