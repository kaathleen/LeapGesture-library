#include "LeapProcess.h"
#include "RecognizedGesture.h"
#include "LeapListener.h"
#include "FileListener.h"

#include "RecognitionModule/FingerDiff.h"

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
	///
	vector<GestureFrame> gesFrame1;
	vector<GestureFrame> gesFrame2;

	GestureStorageDriver *gsd = new BinaryFileStorageDriver();
	cout<<"Stan odczytu gesFrame1: "<<gsd->loadAllGestureFrames("r1.lmr", gesFrame1)<<endl;
	cout<<"Stan odczytu gesFrame2: "<<gsd->loadAllGestureFrames("r2.lmr", gesFrame2)<<endl;

	FingerDiff fingerDiff;
	std::vector<ClassDataset> classDatasets;
	classDatasets.push_back(ClassDataset("klasa1", gesFrame1));
	classDatasets.push_back(ClassDataset("klasa2", gesFrame2));

	cout<<"gesFrame1: "<<gesFrame1.size()<<endl;
	cout<<"gesFrame2: "<<gesFrame2.size()<<endl;

	fingerDiff.train(classDatasets, "/home/kuba", "lolek", true);

	delete gsd;
	///

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
