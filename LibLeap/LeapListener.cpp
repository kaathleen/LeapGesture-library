#include "LeapListener.h"
#include "LMpre.h"

using namespace std;

LeapListener::LeapListener() {}

void LeapListener::attachToProcess(LeapProcess *lp) {
	process = lp;
	frameCount = 0;
}


void LeapListener::onConnect(const Controller& c) {}

void LeapListener::onFrame(const Controller& c) {

	frameCount++;

	if(pthread_mutex_trylock(&process->finished) != 0) return;

	pthread_mutex_trylock(&process->mutexDyn);
	pthread_mutex_trylock(&process->mutexStat);


	convertedFrame = new GestureFrame();

	convert(c.frame());
	cout << "Loading data [" << frameCount << "]" << endl;
	process->loadFrame(convertedFrame);

	pthread_mutex_unlock(&process->mutexStat);
	pthread_mutex_unlock(&process->mutexDyn);

}

void LeapListener::onDisconnect(const Controller& c) {
	cout << "bye." << endl;
}

void LeapListener::convert(Frame inputFrame) {
	lr.prepareData(inputFrame, convertedFrame, 0);
}
