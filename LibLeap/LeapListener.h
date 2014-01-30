#ifndef LEAPLISTENER_H_
#define LEAPLISTENER_H_

#include "LMpre.h"
#include "includes.h"
#include "LMRecorder.h"
#include "LeapProcess.h"
#include <vector>


using namespace Leap;
using namespace std;


class LeapListener : public Listener {


public:

	LMRecorder lr;
	LMpre::LMpre *preprocessor;
	LeapProcess *process;
	GestureFrame *convertedFrame;
	GestureFrame frameObj;
	vector<GestureFrame> frameList;
	vector<GestureFrame> processedFrames;

	LeapListener(int preprocessingRadius);
	void attachToProcess(LeapProcess *lp);
	virtual void onConnect(const Controller& c);
	virtual void onFrame(const Controller& c);
	virtual void onDisconnect(const Controller& c);

private:
	int frameCount;
	int radius;
	void convert(Frame inputFrame);

};

#endif /* LEAPLISTENER_H_ */
