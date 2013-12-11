#include <vector>
#include <iostream>
#include <algorithm>

#include "SVMclassificator.h"

#define MAX_FINGER_COUNT 5

using namespace std;

// This is optimal. Change only in special cases
const int TRAIN_TEST_PERCENT = 50;

vector<double> features1(GestureFrame *gestureFrame, ofstream& svmGesture) {

	GestureHand tempHand = gestureFrame->getHand(0);
	int fingerCount = tempHand.getFingerCount();
	int attributeCounter = 1;

	svmGesture << attributeCounter << ":" << fingerCount << " ";

	//distance between two nearest base points of a finger
	Vertex firstBaseFingerPosition = tempHand.getFinger(0)->getTipPosition() - tempHand.getFinger(0)->getLength()*tempHand.getFinger(0)->getDirection().getNormalized();
	for(int i=1; i<fingerCount; i++)
	{
		GestureFinger tempFinger = tempHand.getFinger(i);
		Vertex baseFingerPosition = tempFinger.getTipPosition() - tempFinger.getLength()*tempFinger.getDirection().getNormalized();
		float distance = sqrt(pow((firstBaseFingerPosition.getX()-baseFingerPosition.getX()),2)+pow((firstBaseFingerPosition.getY()-baseFingerPosition.getY()),2)+((firstBaseFingerPosition.getZ()-baseFingerPosition.getZ()),2));
		attributeCounter++;
		svmGesture << attributeCounter << ":" << distance << " ";
		firstBaseFingerPosition = baseFingerPosition;
	}
	for(int i=0; i<(MAX_FINGER_COUNT-fingerCount);i++)
	{
		attributeCounter++;
		svmGesture << attributeCounter << ":" << 0 << " ";
	}

	//ratio of the finger thickness to the maximal finger thickness
	float fingersThickness [fingerCount];
	float maxFingerThickness = 0;
	for(int i=0; i<fingerCount; i++)
	{
		fingersThickness[i] = tempHand.getFinger(i)->getWidth();
		if(fingersThickness[i]>maxFingerThickness)
			maxFingerThickness = fingersThickness[i];
	}
	for(int i=0; i<fingerCount; i++)
	{
		attributeCounter++;
		svmGesture << attributeCounter << ":" << (float) fingersThickness[i]/maxFingerThickness << " ";
	}
	for(int i=0; i<(MAX_FINGER_COUNT-fingerCount);i++)
	{
		attributeCounter++;
		svmGesture << attributeCounter << ":" << 0 << " ";
	}

	// angles between fingers
	Vertex leftFingerDirection = tempHand.getFinger(0)->getDirection();
	for(int i=1; i<fingerCount; i++)
	{
		Vertex rightFingerDirection = tempHand.getFinger(0)->getDirection();
		attributeCounter++;
		svmGesture << attributeCounter << ":" << (float) fingersThickness[i]/maxFingerThickness << " ";
		leftFingerDirection = rightFingerDirection;
	}
	for(int i=0; i<(MAX_FINGER_COUNT-fingerCount);i++)
	{
		attributeCounter++;
		svmGesture << attributeCounter << ":" << 0 << " ";
	}

		// Defining the set of features
		vector<double> row;
		// 1 feature -> number of fingers
		row.push_back(r->fingerCount);

		svm_gesture << "1:" << r->fingerCount << " ";
w.push_back(r->fingerCount);

		// angles between fingers
		vector<double> angles(15, 0.0);
		for (int i = 0; i < r->fingerCount; i++) {
			double x = r->getFinger(i, DIRX);
			double y = r->getFinger(i, DIRY);
			double z = r->getFinger(i, DIRZ);
			for (int k = i + 1; k < r->fingerCount; k++) {
				double x2 = r->getFinger(k, DIRX);
				double y2 = r->getFinger(k, DIRY);
				double z2 = r->getFinger(k, DIRZ);

				angles.push_back(abs(x * x2 + y * y2 + z * z2));
			}
		}
		sort(angles.begin(), angles.end(), myfunction);

		for (int i = 0; i < 5; i++) {
			row.push_back(angles[i]);
			svm_gesture << i + 2 << ":" << angles[i] << " ";
		}

		// Distances between tips
		vector<double> dist(15, 0.0);

		for (int i = 0; i < r->fingerCount; i++) {
			double x = r->getFinger(i, TIPX);
			double y = r->getFinger(i, TIPY);
			double z = r->getFinger(i, TIPZ);
			for (int k = i + 1; k < r->fingerCount; k++) {
				double x2 = r->getFinger(k, TIPX);
				double y2 = r->getFinger(k, TIPY);
				double z2 = r->getFinger(k, TIPZ);

				double wyn = sqrt(
						(x - x2) * (x - x2) + (y - y2) * (y - y2)
								+ (z - z2) * (z - z2));
				dist.push_back(wyn);
			}
		}
		sort(dist.begin(), dist.end(), myfunction);

		for (int i = 0; i < 5; i++) {
			row.push_back(dist[i]);
			svm_gesture << i + 7 << ":" << dist[i] << " ";
		}
	return row;
}

void createFeaturesDataSets(const int& NUMBER_OF_CLASSES,
		int& classSize[NUMBER_OF_CLASSES], char** argv,
		vector<vector<double> >& featuresInSamples[NUMBER_OF_CLASSES]) {
	cout <<"createFeaturesDataSets - entry"<< endl;

	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();
	ofstream svm_gesture;
	svm_gesture.open("svm_gesture.dat");
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		classSize[i] = 0;
		gestureStorageDriver->openConnection(argv[i + 1], false);
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
		}
		gestureStorageDriver->closeConnection();
	}

	svm_gesture.close();
	delete gestureStorageDriver;

	cout <<"createFeaturesDataSets - end"<< endl;
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


