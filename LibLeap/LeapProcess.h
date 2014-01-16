#ifndef LEAPPROCESS_H_
#define LEAPPROCESS_H_


#include "includes.h"
#include "RecognizedGesture.h"

using namespace Leap;

class LeapListener;

class LeapProcess {
public:

	void (*f)();
	pthread_mutex_t mutexDyn, mutexStat, finished;
	pthread_t dynamicProcess, staticProcess;
	Controller controller;

	LeapProcess(RecognizedGestureListener *rg, bool enableStatic, bool enableDynamic);
	void start();
	void stop();
	void loadFrame(GestureFrame *frame);
	void *staticThread(void);
	void *dynamicThread(void);
	static void *runStatic(void *c);
	static void *runDynamic(void *c);

private:
	RecognizedGestureListener *rg;
	Controller lm;
	GestureFrame *dataFrame;
	bool enableStatic, enableDynamic;
};

#endif /* LEAPPROCESS_H_ */
