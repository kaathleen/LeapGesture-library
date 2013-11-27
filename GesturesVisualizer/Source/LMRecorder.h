#ifndef LMRECORDER_H_INCLUDED
#define LMRECORDER_H_INCLUDED

#include "Header.h"

#include "GestureFrame.h"
#include "LMRecorderListener.h"

class GestureFrame;

#define czas_start()    clock_gettime(CLOCK_REALTIME, &t1)
#define czas_stop()     clock_gettime(CLOCK_REALTIME, &t2)
#define czas_oblicz()   czas = (double)(t2.tv_sec - t1.tv_sec) + 1.e-9*(t2.tv_nsec - t1.tv_nsec)

class LMRecorder : public Leap::Listener {

public:

	Leap::Frame frame;
	Leap::Finger finger;
	int count, diffx, diffy;
	FILE *outfile;
	int i;
    	timespec t1, t2;
    	double czas;
    	long lastTime;
    	
    	vector<LMRecorderListener*> listeners;
    	
    	GestureFrame *currentFrame;
    	
	LMRecorder() {
		initValues();
	}
	
	void initValues();
	
	void onConnect(const Leap::Controller&);

	void onFrame(const Leap::Controller& c);

	void onDisconnect(const Leap::Controller& c);
	
	void startRecording();
	
	void stopRecording();
	
	void saveRecording(string path);
	
	bool getCurrentFrame(GestureFrame &gestureFrame);
	
	void addListener(LMRecorderListener*);
	
	void notifyListeners();
	
	static Leap::Controller& getController() 
	{
		static Leap::Controller s_controller;

		return  s_controller;
	}

private:
	CriticalSection mutex;
};

#endif // LMRECORDER_H_INCLUDED

