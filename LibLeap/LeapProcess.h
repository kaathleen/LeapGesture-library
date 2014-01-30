#ifndef LEAPPROCESS_H_
#define LEAPPROCESS_H_

#include "RecognitionModule/StaticRec.h"
#include "RecognitionModule/FingerDiff.h"

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
	void loadFrameClone(GestureFrame frame);
	void *staticThread(void);
	void *dynamicThread(void);
	static void *runStatic(void *c);
	static void *runDynamic(void *c);
	TestingStaticRecConf *staticConf;

private:
	RecognizedGestureListener *rg;
	Controller lm;
	GestureFrame *dataFrame;
	GestureFrame g;
	bool enableStatic, enableDynamic;
};

#endif /* LEAPPROCESS_H_ */
