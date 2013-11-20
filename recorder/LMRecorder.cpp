#include <iostream>
#include <fstream>
#include "Leap.h"
#include <stdio.h>
#include <time.h>

#define czas_start()    clock_gettime(CLOCK_REALTIME, &t1)
#define czas_stop()     clock_gettime(CLOCK_REALTIME, &t2)
#define czas_oblicz()   czas = (double)(t2.tv_sec - t1.tv_sec) + 1.e-9*(t2.tv_nsec - t1.tv_nsec)

using namespace std;
using namespace Leap;


class SampleListener : public Listener {

public:

	Frame frame;
	Finger finger;
	int count, diffx, diffy;
	FILE *outfile;
	int i;
    timespec t1, t2;
    double czas;
    long lastTime;

	SampleListener() {
		count = 0;
		diffx = 0;
		diffy = 0;
		lastTime = 0;
	}

	virtual void onConnect(const Controller&) {
		outfile = fopen("output.lmr", "w");
		clock_gettime(CLOCK_REALTIME, &t1);
	}

	virtual void onFrame(const Controller& c) {

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
		}

		// if there were any fingers, add newline (avoids empty lines)
		if(frame.fingers().count())
			fprintf(outfile, "\n");


		printf("F: %d T: %dms FPS: %.2f FPS_AVG: %.2f\n", count, (int)(czas), 1000.0/(czas-lastTime), 1000*count/czas);

		lastTime = czas;
		count++;


	}

	virtual void onDisconnect(const Controller& c) {

		fclose(outfile);

	}

};

int main() {

	SampleListener listener;
	Controller controller;

	cout << "Press Enter key to start/stop recording!" << endl;
	cin.get();

	controller.addListener(listener);

	cin.get();

	controller.removeListener(listener);

	return 0;

}

