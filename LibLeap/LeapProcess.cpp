#include "LeapProcess.h"

using namespace std;


LeapProcess::LeapProcess(RecognizedGestureListener *gesture, bool enaStat, bool enaDyn) {

	this->rg = gesture;
	this->mutexStat = PTHREAD_MUTEX_INITIALIZER;
	this->mutexDyn = PTHREAD_MUTEX_INITIALIZER;
	this->finished = PTHREAD_MUTEX_INITIALIZER;


	this->enableStatic = enaStat;
	this->enableDynamic = enaDyn;

}

void LeapProcess::start() {

	pthread_mutex_unlock(&finished);

	if(enableDynamic) {
		pthread_mutex_lock(&mutexStat);
		pthread_create(&staticProcess, NULL, &LeapProcess::runDynamic, this);
	}

	if(enableStatic) {
		pthread_mutex_lock(&mutexDyn);
		pthread_create(&dynamicProcess, NULL, &LeapProcess::runStatic, this);
	}

}

void *LeapProcess::staticThread(void) {

	cout << "[RUNNING] Static processing" << endl;

	StaticRec staticRec;
	TestingStaticRecConf testingStaticRecConf(staticConf->configurationPath, staticConf->configurationName);
	TestingFrame testingFrame;
	TestingResult *testingResult;

	cout << "K starting" << endl;
	pthread_mutex_lock(&mutexStat);

	LogUtil::setQuietMode(true);

	while(1) {
		pthread_mutex_lock(&mutexStat);

		{
			testingFrame.frame = *dataFrame;
			testingResult = staticRec.classify(testingFrame, testingStaticRecConf);

			cout<<"Is recognized: "<<testingResult->recognized<<endl;
			if (testingResult->recognized)
				cout<<"Test result: "<<testingResult->className<<", timestamp: "<<testingResult->frameTimestamp<<endl;
			for (unsigned int j=0; j<testingResult->testClassResults.size(); j++) {
				cout << "Test result for \""
						<< testingResult->testClassResults[j].className << "\" class: "
						<< testingResult->testClassResults[j].classTrainRate << endl;
			}
		}


		pthread_mutex_unlock(&finished);
		if(dataFrame->getHandCount() >= 1 && dataFrame->getHand(0)->getFingerCount() >= 2){

			rg->onStaticRecognized(testingResult);

		}
	}


	return NULL;
}

void *LeapProcess::dynamicThread(void) {

	cout << "[RUNNING] Dynamic processing" << endl;

	// inicjalizacja obiektu przetwarzania równoległego, bla, bla

	// Przetwarzarka p = new Przetwarzarka();


	while(1) {
		pthread_mutex_lock(&mutexDyn);

//		cout << "[DYN] start..." << endl;

		for(int i=0; i<4000; i++) usleep(1000);

		cout << "[DYN] finished..." << endl;
		pthread_mutex_unlock(&finished);
		//rg->onDynamicRecognized(/*typ zwracany?*/);
	}


	return NULL;
}

void *LeapProcess::runStatic(void *c) {
	return ((LeapProcess *)c)->staticThread();
}

void *LeapProcess::runDynamic(void *c) {
	return ((LeapProcess *)c)->dynamicThread();
}

void LeapProcess::stop() {
	pthread_cancel(dynamicProcess);
	pthread_cancel(staticProcess);

}

void LeapProcess::loadFrame(GestureFrame *frame) {
	dataFrame = frame;
}

void LeapProcess::loadFrameClone(GestureFrame frame) {
	g = frame;
	cout << "got new frame" << endl;
}
