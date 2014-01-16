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

	pthread_mutex_lock(&finished);

	if(enableStatic) {
		pthread_mutex_lock(&mutexStat);
		pthread_create(&staticProcess, NULL, &LeapProcess::runDynamic, this);
	}

	if(enableDynamic) {
		pthread_mutex_lock(&mutexDyn);
		pthread_create(&dynamicProcess, NULL, &LeapProcess::runStatic, this);
	}

}

void *LeapProcess::staticThread(void) {

	cout << "[RUNNING] Static processing" << endl;

	// inicjalizacja obiektu przetwarzania równoległego, bla, bla

	// Przetwarzarka p = new Przetwarzarka();


	while(1) {
		pthread_mutex_lock(&mutexStat);

//		cout << "[STA] processing..." << endl;
//		 przetwarzanie

		for(int i=0; i<1000; i++) usleep(1000);

//		cout << "[STA] finished..." << endl;
		pthread_mutex_unlock(&finished);
		if(dataFrame->getHandCount() > 1)
			rg->onStaticRecognized(/*typ zwracany?*/);
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

//		cout << "[DYN] finished..." << endl;
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
