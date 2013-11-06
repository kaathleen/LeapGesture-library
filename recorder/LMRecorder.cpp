#include <iostream>
#include <fstream>
#include "Leap.h"
#include <curses.h>

#define czas_start()    clock_gettime(CLOCK_REALTIME, &t1)
#define czas_stop()             clock_gettime(CLOCK_REALTIME, &t2)
#define czas_oblicz()   czas = (double)(t2.tv_sec - t1.tv_sec) + 1.e-9*(t2.tv_nsec - t1.tv_nsec)

using namespace std;
using namespace Leap;



class SampleListener : public Listener {

public:

	Frame frame;
	Finger finger;
	int count, diffx, diffy;
	ofstream outfile;
	int i;
    timespec t1, t2;
    double czas;


	SampleListener() {
		count = 0;
		diffx = 0;
		diffy = 0;
	}

	virtual void onConnect(const Controller&) {
		cout << "initialized!" << endl;
		initscr();
		cbreak();
		outfile.open("output.lmr");
//		czas_start();
	}

	virtual void onFrame(const Controller& c) {

		frame = c.frame();
		finger = frame.fingers().leftmost();
		mvaddch(140-(int)finger.tipPosition().y, 120+(int)finger.tipPosition().x, '#');
		refresh();

//		czas_oblicz();

//		outfile << czas/1000 << "#";

		for(i=0; i<frame.fingers().count(); i++) {
			finger = frame.fingers()[i];
			outfile << "#" << finger.tipPosition().x << ";" << finger.tipPosition().y << ";" << finger.tipPosition().z;
		}

		if(frame.fingers().count())
			outfile << "\n";

	}

	virtual void onDisconnect(const Controller& c) {

		outfile.close();

	}

};

int main() {

	SampleListener listener;
	Controller controller;

	controller.addListener(listener);

	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	controller.removeListener(listener);
	endwin();

	return 0;

}

