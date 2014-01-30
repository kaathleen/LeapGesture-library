#ifndef LMRECORDER_H_INCLUDED
#define LMRECORDER_H_INCLUDED

#include "Leap.h"
#include "Model/GestureFrame.h"
//#include "LeapListener.h"

class LMRecorder : public Leap::Listener {

public:

	void prepareData(const Leap::Frame frame, GestureFrame *currentFrame, double timestamp);
	GestureFrame prepareDataClone(const Leap::Frame frame, double timestamp);

private:
    	
	float getPointDistanceFromPlane(Vertex point, Vertex planePos, Vertex planeNormal);

};

#endif
