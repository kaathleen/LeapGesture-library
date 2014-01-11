#include "LeapProcess.h"
#include "RecognizedGesture.h"
#include "LeapListener.h"
#include "FileListener.h"
#include "includes.h"

using namespace std;
using namespace Leap;

class MyGestures : public RecognizedGestureListener {

	void onStaticRecognized() {
		cout << "Recognized static gesture!" << endl;
	}

	void onDynamicRecognized() {
			cout << "Recognized dynamic gesture!" << endl;
	}

};


int main(int argc, char **argv) {

	MyGestures *gst = new MyGestures();


	/*
	 *
	// odczyt z Leap Motion
	 *
	 */

	/*
	LeapProcess *process = new LeapProcess(gst, true, true);

	LeapListener listener;
	listener.attachToProcess(process);
	Controller c(listener);

	process->start();
	cin.get();
	c.removeListener(listener);
	*/


	/*
	 *
	// oczyt z pliku
	 *
	 */

	LeapProcess *process = new LeapProcess(gst, true, true);
	FileListener fListener;
	fListener.attachToProcess(process);

	process->start();

	string filePath = "/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/dlon.lmr";
	fListener.readFile(filePath);

	cin.get();

	///////////////////////////// END


	return 0;

}
