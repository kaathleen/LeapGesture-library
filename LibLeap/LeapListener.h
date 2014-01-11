#ifndef LEAPLISTENER_H_
#define LEAPLISTENER_H_

#include "includes.h"
#include "LMRecorder.h"
#include "LeapProcess.h"

using namespace Leap;

class LeapListener : public Listener {

public:

	LMRecorder lr;
	LeapProcess *process;
	GestureFrame *convertedFrame;

	LeapListener();
	void attachToProcess(LeapProcess *lp);
	virtual void onConnect(const Controller& c);
	virtual void onFrame(const Controller& c);
	virtual void onDisconnect(const Controller& c);

private:
	int frameCount;
	void convert(Frame inputFrame);

};

#endif /* LEAPLISTENER_H_ */
