#include "LMRecorder.h"

const std::string LMRecorder::TEMP_PATH = "temp.lmr";

LMRecorder::LMRecorder(GestureStorageDriver *gestureStorageDriver)
{
	this->gestureStorageDriver = gestureStorageDriver;
	initValues();
}

void LMRecorder::initValues() {
	currentFrame = NULL;
	count = 0;
	lastTime = 0;
}

void LMRecorder::onConnect(const Leap::Controller&) {
	gestureStorageDriver->openConnection(TEMP_PATH, true);
	clock_gettime(CLOCK_REALTIME, &t1);
}

void LMRecorder::onFrame(const Leap::Controller& c) {
	ScopedLock frameLock(frameMutex);
	ScopedLock closingLock(closingMutex);
	if (currentFrame !=NULL) {
		delete currentFrame;
		currentFrame = NULL;
	}
	currentFrame = new GestureFrame();
	
	clock_gettime(CLOCK_REALTIME, &t2);
	timestamp = (double)(t2.tv_sec - t1.tv_sec) + 1.e-9*(t2.tv_nsec - t1.tv_nsec);
	timestamp *= 1000;
	
	prepareData(c.frame(), currentFrame, timestamp);
	
	gestureStorageDriver->saveGestureFrame(*currentFrame);

	printf("F: %d T: %dms FPS: %.2f FPS_AVG: %.2f\n", count, (int)(timestamp), 1000.0/(timestamp-lastTime), 1000*count/timestamp);

	lastTime = timestamp;
	count++;

	notifyListeners();
}

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

void LMRecorder::onDisconnect(const Leap::Controller& c) {
}

void LMRecorder::startRecording() {
	initValues();
	printf("Start recording\n");
	getController().addListener(*this);
}

void LMRecorder::stopRecording() {
	ScopedLock closingLock(closingMutex);
	printf("Stop recording\n");
	getController().removeListener(*this);
	gestureStorageDriver->closeConnection();
}

void LMRecorder::saveRecording(string path) {

	if(!(path.size() >= 4 && path[path.size()-4]=='.' && path[path.size()-3]=='l' && path[path.size()-2]=='m' && path[path.size()-1]=='r'))
	{
		path += ".lmr";
	}
	
	std::ifstream  src(TEMP_PATH.c_str(), std::ios::binary);
	std::ofstream  dst(path.c_str(),   std::ios::binary);

	dst << src.rdbuf();
}

bool LMRecorder::getCurrentFrame(GestureFrame &gestureFrame)
{
	{
	      MessageManagerLock mm (Thread::getCurrentThread());
	      if (! mm.lockWasGained())
		      return false;
	}
	
	ScopedLock frameLock(frameMutex);
	if (currentFrame != NULL) {
		gestureFrame = *currentFrame;
		return true;
	}
	return false;
}

void LMRecorder::addListener(LMRecorderListener* listener)
{
	listeners.push_back(listener);
}

void LMRecorder::notifyListeners()
{
	for(vector<LMRecorderListener*>::const_iterator i = listeners.begin(); i != listeners.end(); ++i)
	{
		(*i)->onGestureFrameUpdate();
	}
}

