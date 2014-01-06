#include "LMReader.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include "SVMclassificator.h"


#include "Model/GestureFinger.h"
#include "Model/GestureFrame.h"
#include "Model/GestureHand.h"
#include "Model/Vertex.h"

#include "StorageDriver/BinaryFileStorageDriver.h"
#include "StorageDriver/GestureStorageDriver.h"

#include "PathUtil.h"

#include "preprocess/LMpre.h"

using namespace std;
#define MAX_FINGER_COUNT 5

bool myfunction(double i, double j) {
	return (i > j);
}


void addAttribute(float attributeValue, int& attributeCounter,
		vector<double> &attributes) {
	attributes.push_back(attributeValue);
	attributeCounter++;
}

void fingerCountAttribute(int& fingerCount, int& attributeCounter,
		vector<double>& result) {
	addAttribute(fingerCount, attributeCounter, result);
}

void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result, int sorted = 1) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		vector<float> angles(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getDirection();

			int nextOrAll = min(i+1, fingerCount);
			if (sorted)
			{
				nextOrAll = fingerCount;
			}

			for (int j= i+1 ;j < nextOrAll; j++) {
				Vertex fingerDirection2 = tempHand->getFinger(j)->getDirection();
				float angle = abs(acos(fingerDirection.dotProduct(fingerDirection)));
				angles.push_back(angle);// Can be acos if somebody want it
			}
		}

		if (sorted)
		{
			sort(angles.begin(), angles.end(), std::greater<float>());
		}

		for (int i = 0; i < 5; i++) {
			addAttribute(angles[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

void anglesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result, int sorted = 1) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		vector<float> angles(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getDirection();
			float angle = fingerDirection.dotProduct(tempHand->getPalmNormal());
			angles.push_back(angle);	// Can be acos if somebody want it
		}

		if (sorted)
		{
			sort(angles.begin(), angles.end(), std::greater<float>());
		}
		for (int i = 0; i < 5; i++) {
			addAttribute(angles[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else {
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

void distancesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result, int sorted = 1) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		vector<float> distances(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex tipPosition = tempHand->getFinger(i)->getStabilizedTipPosition();

			int nextOrAll = min(i + 1, fingerCount);
			if (sorted) {
				nextOrAll = fingerCount;
			}
			for (int j= i+1 ;j < nextOrAll; j++) {

				Vertex tipPosition2 = tempHand->getFinger(j)->getStabilizedTipPosition();
				float dist = (tipPosition - tipPosition2).getMagnitude();
				distances.push_back(dist);
			}
		}

		if (sorted) {
			sort(distances.begin(), distances.end(), std::greater<float>());
		}

		for (int i = 0; i < 5; i++) {
			addAttribute(distances[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

void distancesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result, int sorted = 1) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		vector<float> distances(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getStabilizedTipPosition();
			Vertex hand = tempHand->getPalmPosition();
			float dist = (fingerDirection - hand).getMagnitude();
			distances.push_back(dist);
		}

		if (sorted)
		{
			sort(distances.begin(), distances.end(), std::greater<float>());
		}
		for (int i = 0; i < 5; i++) {
			addAttribute(distances[i], attributeCounter, result);
		}
	}
	// There are no fingers in the captured data
	else {
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result);
		}
	}
}

// Computes the feature set
vector<double> computeFeatureSet(GestureFrame *gestureFrame, int version) {
	vector<double> result;

	// Get the numbe of fingers
	GestureHand *tempHand = gestureFrame->getHand(0);
	int fingerCount =
			(tempHand != NULL) ?
					min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	// Adding the finger count to the feature set
	fingerCountAttribute(fingerCount, attributeCounter, result);

	// Versions:
	// 1-> LP, dist 5 fin, 5 angles
	// 2-> LP, dist 5 fin, 5 angles, 5 dist to palms,
	// 3-> LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
	// sorted ones:
	// 4-> LP, dist 5 fin, 5 angles
	// 5-> LP, dist 5 fin, 5 angles, 5 dist to palms,
	// 6-> LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
	int sorted = 0;
	if (version > 3) sorted = 1;

	// Adding the 5 highest angles to the feature set
	anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, sorted);

	// Adding the 5 greatest distances to the feature set
	distancesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result);

	if ((version - 1) % 3 >= 1) {
		distancesFingersPalmAttribute(tempHand, fingerCount, attributeCounter,
				result, sorted);

		if ((version - 1) % 3 > 1) {
			// Adding 5 angles to the palm normal
			anglesFingersPalmAttribute(tempHand, fingerCount, attributeCounter,
					result, sorted);
		}
	}

	return result;
}


// 1 parameter is used feature set
int main(int argc, char **argv) {

	// This is optimal. Change only in special cases
	const int TRAIN_TEST_PERCENT = 67;

	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();

	const int NUMBER_OF_CLASSES = argc - 1;
	// storing the size of each class
	int setSize[NUMBER_OF_CLASSES];
	memset(setSize, 0, sizeof(int) * NUMBER_OF_CLASSES);

	// Reading data in
	vector<GestureFrame> frames;
	for (int i = 2; i < argc; i++) {
		string fileName = getFileNameFromPath(argv[i]);

		gestureStorageDriver->openConnection(argv[i], false);
		cout << "Open connection for " << argv[i] << endl;
		GestureFrame currGestureFrame;

		int k=0;
		while (gestureStorageDriver->loadGestureFrame(currGestureFrame)) {
			frames.push_back(currGestureFrame);
			currGestureFrame.clear();
			k++;
		}
		setSize[i-1] = setSize[i-2] + k;
		gestureStorageDriver->closeConnection();
		cout << "Close connection" << endl;
	}
	delete gestureStorageDriver;

	// Preprocessing
	//LMpre::LMpre pre(frames, 10);
	//frames = pre.process();

	cout<<"After preprocessing"<<endl;

	// For every class we have a matrix:
	//  - in which each row is a row of features for 1 example
	//  - number of rows = number of samples
	vector< vector<double> > feature;
	for (int i = 0; i < frames.size(); i++) {
		vector<double> row;
		row = computeFeatureSet(&frames[i], atoi(argv[1]));
		feature.push_back(row);
	}

	cout<<"Here"<<endl;



	ofstream svm_gesture;
	svm_gesture.open("svm_gesture.dat");
	for (int j = 0; j < feature.size(); j++) {

		// j + 1
		int k=1;
		while (j>=setSize[k]) k++;
		svm_gesture << k << " ";

		for (int i = 0; i < feature[j].size(); i++) {
			svm_gesture << i + 1 << ":" << feature[j][i] << " ";
		}
		svm_gesture << endl;

	//	setSize[j]++;
	}
	svm_gesture.close();

	cout << "Preprocessing" << endl;
	return 0;
	/*int window_size = 5;
	 for (int i=0;i<NUMBER_OF_CLASSES;i++)
	 {
	 for (int j=0;j<setSize[j];j+=window_size)
	 {
	 for (int k=0;k<window_size;k++)
	 {
	 feature[i][j+k];
	 }

	 }
	 cout << setSize[i] << endl;
	 }

	 feature[0].erase( feature[0].begin());
	 */
	cout << "Learning" << endl;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		cout << setSize[i] << endl;
	}

// Separation of data into learning and testing sets
/*	int trainSetSize = 0;
	int testSetSize = 0;

	vector<int> trainLabel, testLabel;
	vector< vector<double> > trainFeatures, testFeatures;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		int class_train_size = (setSize[i] * TRAIN_TEST_PERCENT) / 100;
		int class_test_size = setSize[i] - class_train_size;
		trainSetSize += class_train_size;
		testSetSize += class_test_size;

		for (int j = 0; j < class_train_size; j++) {
			trainLabel.push_back(i + 1);
			trainFeatures.push_back(feature[i][j]);
		}
		for (int j = class_train_size; j < setSize[i]; j++) {
			testLabel.push_back(i + 1);
			testFeatures.push_back(feature[i][j]);
		}
	}

	cout << "Train: " << trainLabel.size() << " " << trainFeatures[0].size()
			<< endl;

	cout << "Prepared test set" << endl;
// Creating an object responsible for learning
	SVMclassificator SVM;

// Learning the classifier
	SVM.train(trainFeatures, trainLabel, NUMBER_OF_CLASSES);

	cout << "Model learned" << endl;
// Predicting the labels on the other TRAIN_TEST_PERCENT
	vector<int> pred = SVM.classify(testFeatures);

// Counting the number of correctly recognized labels
	int count = 0;
	for (int i = 0; i < testSetSize; i++) {
		if (pred[i] == testLabel[i]) {
			count++;
		}
	}

// Printing results
	cout << endl << "-------- Result --------" << endl;
	cout << "Recognition rate [%] : " << (count * 1.0 / testSetSize * 100.0)
			<< "%" << endl;
	cout << "Correctly recognized [number] : " << count << endl;
	cout << "Total size of test set [number] : " << testSetSize << endl;*/

	return 0;
}

