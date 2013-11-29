#include "LMRecorder.h"
	
void LMRecorder::initValues() {
	currentFrame = NULL;
	count = 0;
	diffx = 0;
	diffy = 0;
	lastTime = 0;
}

void LMRecorder::onConnect(const Leap::Controller&) {
	outfile = fopen("temp.lmr", "w");
	clock_gettime(CLOCK_REALTIME, &t1);
}

void LMRecorder::onFrame(const Leap::Controller& c) {
	ScopedLock renderLock(mutex);
	if (currentFrame !=NULL) {
		delete currentFrame;
	}
	currentFrame = new GestureFrame();
	
	frame = c.frame();
	finger = frame.fingers().leftmost();

	clock_gettime(CLOCK_REALTIME, &t2);
	czas = (double)(t2.tv_sec - t1.tv_sec) + 1.e-9*(t2.tv_nsec - t1.tv_nsec);
	czas *= 1000;

	if(frame.fingers().count())
		fprintf(outfile, "%.4f", czas);

	// add all fingers
	for(i=0; i<frame.fingers().count(); i++) {
		finger = frame.fingers()[i];
		fprintf(outfile, "#%f;%f;%f;", finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
		fprintf(outfile, "%f;%f;%f", finger.direction().x, finger.direction().y, finger.direction().z);
		
		Vertex fingerTipPos(finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
		Vertex fingerTipVect(finger.direction().x, finger.direction().y, finger.direction().z);
		currentFrame->addFinger(fingerTipPos, fingerTipVect);
	}

	// if there were any fingers, add newline (avoids empty lines)
	if(frame.fingers().count())
		fprintf(outfile, "\n");


	printf("F: %d T: %dms FPS: %.2f FPS_AVG: %.2f\n", count, (int)(czas), 1000.0/(czas-lastTime), 1000*count/czas);

	lastTime = czas;
	count++;

	notifyListeners();
}

void LMRecorder::onDisconnect(const Leap::Controller& c) {
	fclose(outfile);

}

void LMRecorder::startRecording() {
	initValues();
	printf("Start recording\n");
	getController().addListener(*this);
}

void LMRecorder::stopRecording() {
	printf("Stop recording\n");
	getController().removeListener(*this);
}

void LMRecorder::saveRecording(string path) {

	if(!(path.size() >= 4 && path[path.size()-4]=='.' && path[path.size()-3]=='l' && path[path.size()-2]=='m' && path[path.size()-1]=='r'))
	{
		path += ".lmr";
	}
	
	std::ifstream  src("temp.lmr", std::ios::binary);
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
	
	ScopedLock renderLock(mutex);
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

