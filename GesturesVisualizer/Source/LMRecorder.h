#ifndef LMRECORDER_H_INCLUDED
#define LMRECORDER_H_INCLUDED

#include <fstream>

#include "Header.h"

#include "Model/GestureFrame.h"
#include "StorageDriver/GestureStorageDriver.h"

#include "LMRecorderListener.h"

class GestureFrame;

class LMRecorder : public Leap::Listener {

public:
	
	LMRecorder(GestureStorageDriver*);
	
	void onConnect(const Leap::Controller&);

	void onFrame(const Leap::Controller& c);

	void onDisconnect(const Leap::Controller& c);
	
	void startRecording();
	
	void stopRecording();
	
	void saveRecording(string path);
	
	bool getCurrentFrame(GestureFrame &gestureFrame);
	
	void addListener(LMRecorderListener*);
	
	static Leap::Controller& getController() 
	{
		static Leap::Controller s_controller;

		return  s_controller;
	}

private:
	const static std::string TEMP_PATH;

	GestureStorageDriver *gestureStorageDriver;

	CriticalSection frameMutex;
	CriticalSection closingMutex;
	
	int count;
    	timespec t1, t2;
    	double timestamp;
    	long lastTime;
    	
    	vector<LMRecorderListener*> listeners;
    	GestureFrame *currentFrame;
    	
    	void initValues();
    	void prepareData(const Leap::Frame frame, GestureFrame *currentFrame, double timestamp);
    	float getPointDistanceFromPlane(Vertex point, Vertex planePos, Vertex planeNormal);
    	void notifyListeners();
};

#endif // LMRECORDER_H_INCLUDED

