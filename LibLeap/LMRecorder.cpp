#include "LMRecorder.h"


void LMRecorder::prepareData(const Leap::Frame frame, GestureFrame *currFrame, double timestamp)
{
	currFrame->setTimestamp(timestamp);	

	Leap::HandList handsInFrame = frame.hands();
	for(int handIndex=0; handIndex<handsInFrame.count(); handIndex++)
	{
		Leap::Hand currHand = handsInFrame[handIndex];
		
		//create GestureHand
		GestureHand gestureHand(
			currHand.id(),
			Vertex(currHand.palmPosition().x, currHand.palmPosition().y, currHand.palmPosition().z),
			Vertex(0, 0, 0/*currHand.stabilizedPalmPosition().x, currHand.stabilizedPalmPosition().y, currHand.stabilizedPalmPosition().z*/),
			Vertex(currHand.palmNormal().x, currHand.palmNormal().y, currHand.palmNormal().z),
			Vertex(currHand.direction().x, currHand.direction().y, currHand.direction().z)
		);
		gestureHand.setOrderValue(currHand.palmPosition().x);
		
		Vertex planeNormalVec = gestureHand.getDirection().crossProduct(gestureHand.getPalmNormal()).getNormalized();
		
		Leap::FingerList fingersInCurrHand = currHand.fingers();
		for (int fingerIndex=0; fingerIndex<fingersInCurrHand.count(); fingerIndex++)
		{
			Leap::Finger currFinger = fingersInCurrHand[fingerIndex];
		
			Leap::Vector leapFingerTipPos = currFinger.tipPosition();
			Vertex fingerTipPos(leapFingerTipPos.x, leapFingerTipPos.y, leapFingerTipPos.z);
			float distance = getPointDistanceFromPlane(fingerTipPos, gestureHand.getPalmPosition(), planeNormalVec);
			
			//create GestureFinger
			GestureFinger gestureFinger(
				currFinger.id(),
				fingerTipPos,
				Vertex(currFinger.stabilizedTipPosition().x, currFinger.stabilizedTipPosition().y, currFinger.stabilizedTipPosition(). z),
				Vertex(currFinger.direction().x, currFinger.direction().y, currFinger.direction().z),
				currFinger.length(),
				currFinger.width()
			);
			gestureFinger.setOrderValue(distance);
			
			gestureHand.addFinger(gestureFinger);
		}		
		gestureHand.sortFingers();
		
		currFrame->addHand(gestureHand);
	}
	currFrame->sortHands();
}

float LMRecorder::getPointDistanceFromPlane(Vertex point, Vertex planePos, Vertex planeNormal)
{
	return -(planeNormal.dotProduct(point - planePos));
}
