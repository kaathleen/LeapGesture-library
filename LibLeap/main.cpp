#include "LeapProcess.h"
#include "RecognizedGesture.h"
#include "LeapListener.h"
#include "FileListener.h"

#include "includes.h"

using namespace std;
using namespace Leap;

void testFingerDiff();
void testStaticRec();
void nauczaj();

class MyGestures: public RecognizedGestureListener {

	void onStaticRecognized(TestingResult *tr) {

		if(tr->testClassResults[0].classTrainRate > 0.5) {
			cout << "CLASS 0!" << endl;
		}
			else cout << "CLASS 1!" << endl;
	}

	void onDynamicRecognized() {
		cout << "Recognized dynamic gesture!" << endl;
	}

};

int main(int argc, char **argv) {

//	nauczaj();
//	return 3;

//	vector<pair<const char*, const char*> > input;
//	input.push_back(make_pair("/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/moje/szatan.lmr", "szatan"));
//	input.push_back(make_pair("/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/moje/peace.lmr", "peace"));
//	Learning::learnStatic(input, "/home/oli", "test2");



	MyGestures *gst = new MyGestures();

	/*
	*
	// odczyt z Leap Motion
	*
	*/
	LeapProcess *process = new LeapProcess(gst, true, false);

	TestingStaticRecConf settings("/home/oli", "test2");
	process->staticConf = &settings;

	LeapListener listener(3);
	listener.attachToProcess(process);
	Controller c(listener);

	process->start();
	cin.get();

	c.removeListener(listener);

	/*
	 *
	 // oczyt z pliku
	 *
	 */

//	LeapProcess *process = new LeapProcess(gst, true, true);
//	FileListener fListener;
//	fListener.attachToProcess(process);
//
//	process->start();
//
//	string filePath =
//			"/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/moje/szatan.lmr";
//	fListener.readFile(filePath);
//
//	cin.get();

	///////////////////////////// END

	return 0;
}

void nauczaj() {

	cout << "START STATIC REC TEST";

	StaticRec staticRec;

	LogUtil::setQuietMode(false);

	GestureStorageDriver *gsd = new BinaryFileStorageDriver();
	std::vector<TrainingClassDataset> classDatasets;

	//training
	vector<GestureFrame> gesFrame1;
	vector<GestureFrame> gesFrame2;
	gsd->loadAllGestureFrames("/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/moje/peace.lmr", gesFrame1);
	gsd->loadAllGestureFrames("/home/oli/workspace/projekt-inzynierski/GesturesVisualizer/Builds/Linux/build/moje/szatan.lmr", gesFrame2);
	classDatasets.push_back(TrainingClassDataset("klasa1", gesFrame1));
	classDatasets.push_back(TrainingClassDataset("klasa2", gesFrame2));


	TrainingStaticRecConf trainingStaticRecConf("/home/oli", "lolek");
	TrainingResult* trainResult = staticRec.train(classDatasets, trainingStaticRecConf);

	cout << "Train result: " << trainResult->trainRate << endl;
	for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
		cout << "Train result for \""
				<< trainResult->trainClassResults[i].className << "\" class: "
				<< trainResult->trainClassResults[i].classTrainRate << endl;
	}


}

void testStaticRec() {
	cout << "START STATIC REC TEST";

	StaticRec staticRec;

	LogUtil::setQuietMode(false);

	vector<GestureFrame> gesFrame1;
	vector<GestureFrame> gesFrame2;

	GestureStorageDriver *gsd = new BinaryFileStorageDriver();

	//training
	cout << "Stan odczytu gesFrame1: "
			<< gsd->loadAllGestureFrames("r1.lmr", gesFrame1) << endl;
	cout << "Stan odczytu gesFrame2: "
			<< gsd->loadAllGestureFrames("r2.lmr", gesFrame2) << endl;

	std::vector<TrainingClassDataset> classDatasets;
	classDatasets.push_back(TrainingClassDataset("klasa1", gesFrame1));
	classDatasets.push_back(TrainingClassDataset("klasa2", gesFrame2));

	cout << "gesFrame1: " << gesFrame1.size() << endl;
	cout << "gesFrame2: " << gesFrame2.size() << endl;

	TrainingStaticRecConf trainingStaticRecConf("/home/oli", "lolek");
	TrainingResult* trainResult = staticRec.train(classDatasets, trainingStaticRecConf);

	cout << "Train result: " << trainResult->trainRate << endl;
	for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
		cout << "Train result for \""
				<< trainResult->trainClassResults[i].className << "\" class: "
				<< trainResult->trainClassResults[i].classTrainRate << endl;
	}

	//testing
	vector<GestureFrame> frames;
	cout << "Stan odczytu testFrames: " << gsd->loadAllGestureFrames("tests.lmr", frames) << endl;
	TestingStaticRecConf testingStaticRecConf("/home/oli", "lolek");
	for (unsigned int i=0; i<frames.size(); i++) {
		TestingFrame testingFrame(frames[i]);
		TestingResult *testingResult = staticRec.classify(testingFrame, testingStaticRecConf);

		cout<<"Is recognized: "<<testingResult->recognized<<endl;
		if (testingResult->recognized)
			cout<<"Test result: "<<testingResult->className<<", timestamp: "<<testingResult->frameTimestamp<<endl;
		for (unsigned int j=0; j<testingResult->testClassResults.size(); j++) {
			cout << "Test result for \""
					<< testingResult->testClassResults[j].className << "\" class: "
					<< testingResult->testClassResults[j].classTrainRate << endl;
		}
	}

	//

	delete gsd;

	cout << "END STATIC REC TEST";
}

void testFingerDiff() {
	cout << "START FINGER DIFF TEST";

	FingerDiff fingerDiff;

	LogUtil::setQuietMode(false);

	vector<GestureFrame> gesFrame1;
	vector<GestureFrame> gesFrame2;

	GestureStorageDriver *gsd = new BinaryFileStorageDriver();

	//training
	cout << "Stan odczytu gesFrame1: "
			<< gsd->loadAllGestureFrames("r1.lmr", gesFrame1) << endl;
	cout << "Stan odczytu gesFrame2: "
			<< gsd->loadAllGestureFrames("r2.lmr", gesFrame2) << endl;

	std::vector<TrainingClassDataset> classDatasets;
	classDatasets.push_back(TrainingClassDataset("klasa1", gesFrame1));
	classDatasets.push_back(TrainingClassDataset("klasa2", gesFrame2));

	cout << "gesFrame1: " << gesFrame1.size() << endl;
	cout << "gesFrame2: " << gesFrame2.size() << endl;

	TrainingFingerDiffConf trainingFingerDiffConf("/home/kuba", "lolek");
	TrainingResult* trainResult = fingerDiff.train(classDatasets, trainingFingerDiffConf);

	cout << "Train result: " << trainResult->trainRate << endl;
	for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
		cout << "Train result for \""
				<< trainResult->trainClassResults[i].className << "\" class: "
				<< trainResult->trainClassResults[i].classTrainRate << endl;
	}

	//testing
	vector<GestureFrame> frames;
	cout << "Stan odczytu testFrames: "
			<< gsd->loadAllGestureFrames("tests.lmr", frames) << endl;
	for (unsigned int i=0; i<frames.size(); i++) {
		TestingFrame testingFrame(frames[i]);
		TestingFingerDiffConf testingFingerDiffConf("/home/kuba", "lolek", 0.5);
		TestingResult *testingResult = fingerDiff.classify(testingFrame, testingFingerDiffConf);

		cout<<"Is recognized: "<<testingResult->recognized<<endl;
		if (testingResult->recognized)
			cout<<"Test result: "<<testingResult->className<<", timestamp: "<<testingResult->frameTimestamp<<endl;
		for (unsigned int j=0; j<testingResult->testClassResults.size(); j++) {
			cout << "Test result for \""
					<< testingResult->testClassResults[j].className << "\" class: "
					<< testingResult->testClassResults[j].classTrainRate << endl;
		}
	}

	//

	delete gsd;

	cout << "END FINGER DIFF TEST";
}
