#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

#include "Preprocessing/LMpre.h"

#include "Model/GestureFinger.h"
#include "Model/GestureFrame.h"
#include "Model/GestureHand.h"
#include "Model/Vertex.h"

#include "StorageDriver/BinaryFileStorageDriver.h"
#include "StorageDriver/GestureStorageDriver.h"

#include "PathUtil.h"

#include <vector>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

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
	addAttribute(fingerCount, attributeCounter, result, svmGesture);
}

void nearestFingersDistancesAttribute(GestureHand* tempHand, int fingerCount,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
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
					abs((firstBaseFingerPosition - baseFingerPosition).getMagnitude());
			addAttribute(distance, attributeCounter, result, svmGesture);
			firstBaseFingerPosition = baseFingerPosition;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

void nearestFingersDistancesRatiosAttribute(GestureHand* tempHand, int fingerCount,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
	//distance between two nearest base points of a finger
	if (tempHand != NULL && fingerCount > 1) {
		float minimalDistance = 0;
		vector<double> distances;
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
					abs((firstBaseFingerPosition - baseFingerPosition).getMagnitude());
			if ((minimalDistance == 0 || distance < minimalDistance) && distance > 0)
			{
				minimalDistance = distance;
			}

			distances.push_back(distance);
			firstBaseFingerPosition = baseFingerPosition;
		}
		for (int i = 0; i < distances.size(); i++) {
			float distanceRatio = 0;
			if (minimalDistance > 0)
				distanceRatio = distances[i] / minimalDistance;

			addAttribute(distanceRatio, attributeCounter, result, svmGesture);
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, svmGesture);
	}
}

void fingerThicknessRatiosAttribute(int& fingerCount, GestureHand* tempHand,
		int& attributeCounter, vector<double>& result, ofstream& svmGesture) {
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

	//ratio of distance between two nearest base points of a finger to the minimal (non-zero) distance between two nearest base points
	/*nearestFingersDistancesRatiosAttribute(tempHand, fingerCount, attributeCounter,
			result, svmGesture);*/

	//ratio of the finger thickness to the maximal finger thickness
	fingerThicknessRatiosAttribute(fingerCount, tempHand, attributeCounter,
			result, svmGesture);

	// angles between fingers
	/*anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, svmGesture);*/

	// angles between finger and first finger relative to palmPosition
	anglesBetweenFingersRelativeToPalmPosAttribute(tempHand, fingerCount,
			attributeCounter, result, svmGesture);

	return result;
}

int getIndexOfClassName(vector<string> &classNames, string name)
{
	for (int i=0; i<classNames.size(); i++)
	{
		if (classNames[i].compare(name) == 0)
			return i;
	}

	return -1;
}

std::vector<GestureFrame> preprocessGestureFrames(vector<GestureFrame>& gestureFrameVector)
{
	LMpre::LMpre preprocessing(gestureFrameVector, 4);
	vector < GestureFrame > preGestureFrameVector;
	preGestureFrameVector = preprocessing.process();
	return preGestureFrameVector;
}

void createFeaturesDataSets(const int& NUMBER_OF_CLASSES, char** argv,
		vector<vector<vector<double> > > &featuresInSamplesInClasses,
		vector<string> &classNames) {
	std::cout << "createFeaturesDataSets - entry" << std::endl;

	GestureStorageDriver* gestureStorageDriver = new BinaryFileStorageDriver();
	ofstream svm_gesture;
	svm_gesture.open("svm_gesture.dat");
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		string fileName = getFileNameFromPath(argv[i + 1]);
		cout<<"<"<<fileName<<"> ";
		int classNameIndex = getIndexOfClassName(classNames, fileName);
		if (classNameIndex < 0)
		{
			classNames.push_back(fileName);
			classNameIndex = classNames.size()-1;
			featuresInSamplesInClasses.push_back(vector<vector<double> >());
		}
		std::vector<GestureFrame> gestureFrameVector;
		gestureStorageDriver->loadAllGestureFrames(argv[i + 1], gestureFrameVector);
		std::vector < GestureFrame > preGestureFrameVector = gestureFrameVector;// preprocessGestureFrames(gestureFrameVector);
		//gestureStorageDriver->openConnection(argv[i + 1], false);
		//cout << "Open connection for "<<argv[i + 1]<< endl;
		//GestureFrame currGestureFrame;
		//while (gestureStorageDriver->loadGestureFrame(currGestureFrame)) {
		for(int i=0; i<preGestureFrameVector.size(); i++)
		{
			// Defining the set of features
			vector<double> row;
			// sequence number of class
			svm_gesture << classNameIndex + 1 << " ";
			row = features1(&preGestureFrameVector[i], svm_gesture);
			svm_gesture << endl;

			featuresInSamplesInClasses[classNameIndex].push_back(row);

			//currGestureFrame.clear();
		}
		//gestureStorageDriver->closeConnection();
		//cout << "Close connection" << endl;
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
	const int NUMBER_OF_FILES = argc - 1;

	// For every class we have a matrix:
	//  - in which each row is a row of features for 1 example
	//  - number of rows = number of samples
	vector<vector<vector<double> > > featuresInSamplesInClasses;

	vector<string> classNames;

	// Create features data sets
	createFeaturesDataSets(NUMBER_OF_FILES, argv,
			featuresInSamplesInClasses, classNames);

	return 0;
}

