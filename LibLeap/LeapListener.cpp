#include "LeapListener.h"


using namespace std;

LeapListener::LeapListener(int preprocessingRadius) {

	radius = preprocessingRadius;
	preprocessor = new LMpre::LMpre(frameList, radius);
}

void LeapListener::attachToProcess(LeapProcess *lp) {
	process = lp;
	frameCount = 0;
}


void LeapListener::onConnect(const Controller& c) {

	cout << "Processing started!" << endl;

}

void LeapListener::onFrame(const Controller& c) {

	frameCount++;

	convertedFrame = new GestureFrame();
	convert(c.frame());

	if(frameList.size() < radius*2+1) {
		frameList.push_back(*convertedFrame);
		return;
	}

	if(pthread_mutex_trylock(&process->finished) != 0) {
		cout << "dropping..." << endl;
		return;
	}

	frameList.erase(frameList.begin());
	frameList.push_back(*convertedFrame);

	cout << "FrameList size: " << frameList.size() << endl;


	delete preprocessor;
	preprocessor = new LMpre::LMpre(frameList, radius);
	preprocessor->frames = frameList;
	processedFrames = preprocessor->process();

	pthread_mutex_trylock(&process->mutexDyn);
	pthread_mutex_trylock(&process->mutexStat);

	cout << "Preprocessing done!" << endl;
	cout << processedFrames.size() << endl;

//	process->loadFrame(&processedFrames[radius]);
	process->loadFrame(&frameList[radius]);

	pthread_mutex_unlock(&process->mutexStat);
	pthread_mutex_unlock(&process->mutexDyn);

}

void LeapListener::onDisconnect(const Controller& c) {
	cout << "bye." << endl;
}

void LeapListener::convert(Frame inputFrame) {
	lr.prepareData(inputFrame, convertedFrame, 0);
}
