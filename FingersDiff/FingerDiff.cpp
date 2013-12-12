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

using namespace std;

// This is optimal. Change only in special cases
const int TRAIN_TEST_PERCENT = 50;

const int MAX_FINGER_COUNT = 5;

void addAttribute(float attributeValue, int& attributeCounter,
		vector<double> &attributes, ofstream& attributeStream) {
	attributes.push_back(attributeValue);
	attributeStream << attributeCounter << ":" << attributeValue << " ";
	attributeCounter++;
}

void fingerCountAttribute(int& fingerCount, int& attributeCounter,
		vector<double>& result, ofstream& svmGesture) {
	svmGesture<<"FCA";
	addAttribute(fingerCount, attributeCounter, result, svmGesture);
}

void nearestFingersDistancesAttribute(GestureHand* tempHand, int fingerCount,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
	svmGesture<<"NFDA";
	//distance between two nearest base points of a finger
	if (tempHand != NULL && fingerCount > 1) {
		Vertex firstBaseFingerPosition =
				tempHand->getFinger(0)->getTipPosition()
						- tempHand->getFinger(0)->getDirection().getNormalized()
								* tempHand->getFinger(0)->getLength();
		for (int i = 1; i < fingerCount; i++) {
			GestureFinger *tempFinger = tempHand->getFinger(i);
			Vertex baseFingerPosition = tempFinger->getTipPosition()
					- tempFinger->getDirection().getNormalized()
							* tempFinger->getLength();
			float distance =
					(firstBaseFingerPosition - baseFingerPosition).getMagnitude();
			addAttribute(distance, attributeCounter, result, svmGesture);
			firstBaseFingerPosition = baseFingerPosition;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

void fingerThicknessRatiosAttribute(int& fingerCount, GestureHand* tempHand,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
	svmGesture<<"FTRA";
	//ratio of the finger thickness to the maximal finger thickness
	float fingersThickness[fingerCount];
	float maxFingerThickness = 0;
	for (int i = 0; i < fingerCount; i++) {
		fingersThickness[i] = tempHand->getFinger(i)->getWidth();
		if (fingersThickness[i] > maxFingerThickness)
			maxFingerThickness = fingersThickness[i];
	}
	for (int i = 0; i < fingerCount; i++) {
		int fingerThicknessRatio =
				(maxFingerThickness != 0) ?
						(float) (((fingersThickness[i]))) / maxFingerThickness :
						0;
		addAttribute(fingerThicknessRatio, attributeCounter, result,
				svmGesture);
	}
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
	svmGesture<<"ABFA";
	// angles between fingers
	if (tempHand != NULL && fingerCount > 1) {
		Vertex leftFingerDirection = tempHand->getFinger(0)->getDirection();
		for (int i = 1; i < fingerCount; i++) {
			Vertex rightFingerDirection =
					tempHand->getFinger(i)->getDirection();

			float angle =
					abs(
							acos(
									leftFingerDirection.dotProduct(
											rightFingerDirection)
											/ (leftFingerDirection.getMagnitude()
													* rightFingerDirection.getMagnitude())));
			addAttribute(angle, attributeCounter, result, svmGesture);
			leftFingerDirection = rightFingerDirection;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

void anglesBetweenFingersRelativeToPalmPosAttribute(GestureHand* tempHand,
		int fingerCount, int attributeCounter, vector<double>& result,
		ofstream& svmGesture) {
	svmGesture<<"ABFRTPPA";
	// angles between finger and first finger relative to palmPosition
	if (tempHand != NULL && fingerCount > 1) {
		Vertex leftBaseFingerPalmPosition =
				(tempHand->getFinger(0)->getTipPosition()
						- tempHand->getFinger(0)->getDirection().getNormalized()
								* tempHand->getFinger(0)->getLength())
						- tempHand->getPalmPosition();
		for (int i = 1; i < fingerCount; i++) {
			GestureFinger *tempFinger = tempHand->getFinger(i);
			Vertex rightBaseFingerPalmPosition = (tempFinger->getTipPosition()
					- tempFinger->getDirection().getNormalized()
							* tempFinger->getLength())
					- tempHand->getPalmPosition();
			float angle =
					abs(
							acos(
									leftBaseFingerPalmPosition.dotProduct(
											rightBaseFingerPalmPosition)
											/ (leftBaseFingerPalmPosition.getMagnitude()
													* rightBaseFingerPalmPosition.getMagnitude())));
			addAttribute(angle, attributeCounter, result, svmGesture);
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

vector<double> features1(GestureFrame *gestureFrame, ofstream& svmGesture) {

	vector<double> result;
	GestureHand *tempHand = gestureFrame->getHand(0);
	int fingerCount =
			(tempHand != NULL) ?
					min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	//fingerCount
	fingerCountAttribute(fingerCount, attributeCounter, result, svmGesture);

	//distance between two nearest base points of a finger
	/*nearestFingersDistancesAttribute(tempHand, fingerCount, attributeCounter,
			result, svmGesture);*/

	//ratio of the finger thickness to the maximal finger thickness
	/*fingerThicknessRatiosAttribute(fingerCount, tempHand, attributeCounter,
			result, svmGesture);*/

	// angles between fingers
	/*anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, svmGesture);*/

	// angles between finger and first finger relative to palmPosition
	/*anglesBetweenFingersRelativeToPalmPosAttribute(tempHand, fingerCount,
			attributeCounter, result, svmGesture);*/

	return result;
}

void createFeaturesDataSets(const int& NUMBER_OF_CLASSES,
		int *classSize, char** argv,
		vector<vector<double> > *featuresInSamples) {
	cout << "createFeaturesDataSets - entry" << endl;

	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();
	ofstream svm_gesture;
	svm_gesture.open("svm_gesture.dat");
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		classSize[i] = 0;
		gestureStorageDriver->openConnection(argv[i + 1], false);
		cout << "Open connection for "<<argv[i + 1]<< endl;
		GestureFrame currGestureFrame;
		while (gestureStorageDriver->loadGestureFrame(currGestureFrame)) {
			// Defining the set of features
			vector<double> row;
			// sequence number of class
			svm_gesture << i + 1 << " ";
			row = features1(&currGestureFrame, svm_gesture);
			svm_gesture << endl;
			featuresInSamples[i].push_back(row);
			classSize[i]++;

			currGestureFrame.clear();
		}
		gestureStorageDriver->closeConnection();
		cout << "Close connection" << endl;
	}

	svm_gesture.close();
	delete gestureStorageDriver;

	cout << "createFeaturesDataSets - end" << endl;
}

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Usage: %s file_class1 file_class2 ...\n", argv[0]);
		exit(1);
	}
	const int NUMBER_OF_CLASSES = argc - 1;

	// For every class we have a matrix:
	//  - in which each row is a row of features for 1 example
	//  - number of rows = number of samples
	vector<vector<double> > featuresInSamples[NUMBER_OF_CLASSES];

	// Storing the size of each class
	int classSize[NUMBER_OF_CLASSES];

	// Create features data sets
	createFeaturesDataSets(NUMBER_OF_CLASSES, classSize, argv,
			featuresInSamples);

	cout << "Preprocessing" << endl;
	//return 0;
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
		cout << classSize[i] << endl;
	}

	// Separation of data into learning and testing sets
	int trainSetSize = 0;
	int testSetSize = 0;

	vector<int> trainLabel, testLabel;
	vector<vector<double> > trainFeatures, testFeatures;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		int class_train_size = (classSize[i] * TRAIN_TEST_PERCENT) / 100;
		int class_test_size = classSize[i] - class_train_size;
		trainSetSize += class_train_size;
		testSetSize += class_test_size;

		for (int j = 0; j < class_train_size; j++) {
			trainLabel.push_back(i + 1);
			trainFeatures.push_back(featuresInSamples[i][j]);
		}
		for (int j = class_train_size; j < classSize[i]; j++) {
			testLabel.push_back(i + 1);
			testFeatures.push_back(featuresInSamples[i][j]);
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
	cout << "Total size of test set [number] : " << testSetSize << endl;

	return 0;
}

