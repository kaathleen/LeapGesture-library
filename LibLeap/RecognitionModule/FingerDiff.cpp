#include "FingerDiff.h"

const int FingerDiff::MAX_FINGER_COUNT = 5;

FingerDiff::FingerDiff()
{
}

TrainResult FingerDiff::train(ClassDatasets& classDatasets, std::string configurationPath, std::string configurationName, bool saveDatasetFile)
{
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->saveDatasetFile = saveDatasetFile;

	createGenericClassNames(classDatasets);
	saveGenericClassNames();

	std::vector<std::vector<double> > trainDataset; //features in samples
	std::vector<int> trainLabels;
	createFeaturesDataSet(classDatasets, trainDataset, trainLabels);

	//SVMclassificator svm;
	//svm.train(trainDataset, trainLabels, genericClassNames.size(), configurationPath, configurationName);

	TrainResult trainResult;
	return trainResult;
}

void FingerDiff::createGenericClassNames(std::vector<ClassDataset>& classDatasets)
{
	for (uint i=0; i<classDatasets.size(); i++)
	{
		std::string className = classDatasets[i].className;
		bool isNewClass = true;
		uint j;
		for (j=0; j<genericClassNames.size(); j++)
		{
			if (genericClassNames[j] == className)
			{
				isNewClass = false;
				break;
			}
		}

		if (isNewClass)
		{
			genericClassNames.push_back(className);
			classDatasets[i].genericClassName = genericClassNames.size()-1;
		}
		else
		{
			classDatasets[i].genericClassName = j;
		}
	}
}

void FingerDiff::saveGenericClassNames()
{
	std::string genericClassFilePath = PathUtil::combinePathFileNameAndExt(configurationPath, configurationName, ConfExt::CLASS_MAP_EXT);
	FileWriterUtil genericClassFile(genericClassFilePath);
	genericClassFile.open();
	for (int i=0; i<genericClassNames.size(); i++)
	{
		genericClassFile<<genericClassNames[i]<<"\n";
	}
	genericClassFile.close();
}

void FingerDiff::createFeaturesDataSet(std::vector<ClassDataset>& classDatasets,
		std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels) {
	std::string datasetFilePath = PathUtil::combinePathFileNameAndExt(configurationPath, configurationName, ConfExt::DAT_EXT);
	FileWriterUtil datasetFile(datasetFilePath, !saveDatasetFile);
	datasetFile.open();

	for (uint i = 0; i < classDatasets.size(); i++)
	{
		for(uint j=0; j<classDatasets[i].dataset.size(); j++)
		{
			// Defining the set of features
			std::vector<double> row;
			// sequence number of class
			datasetFile<<classDatasets[i].genericClassName<<" ";
			row = createFeatures(&(classDatasets[i].dataset[j]), datasetFile);
			datasetFile<<"\n";

			trainDataset.push_back(row);
			trainLabels.push_back(classDatasets[i].genericClassName);
		}
	}

	datasetFile.close();
}

std::vector<double> FingerDiff::createFeatures(GestureFrame *gestureFrame, FileWriterUtil& datasetFile) {
	std::vector<double> result;
	GestureHand *tempHand = gestureFrame->getHand(0);
	int fingerCount = (tempHand != NULL) ? std::min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	//fingerCount
	fingerCountAttribute(fingerCount, attributeCounter, result, datasetFile);

	//distance between two nearest base points of a finger
	/*nearestFingersDistancesAttribute(tempHand, fingerCount, attributeCounter,
			result, svmGesture);*/

	//ratio of distance between two nearest base points of a finger to the minimal (non-zero) distance between two nearest base points
	nearestFingersDistancesRatiosAttribute(tempHand, fingerCount, attributeCounter,
			result, datasetFile);

	//ratio of the finger thickness to the maximal finger thickness
	fingerThicknessRatiosAttribute(fingerCount, tempHand, attributeCounter,
			result, datasetFile);

	// angles between fingers
	anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, datasetFile);

	// angles between finger and first finger relative to palmPosition
	anglesBetweenFingersRelativeToPalmPosAttribute(tempHand, fingerCount,
			attributeCounter, result, datasetFile);

	return result;
}

void FingerDiff::fingerCountAttribute(int& fingerCount, int& attributeCounter,
		std::vector<double>& result, FileWriterUtil& datasetFile) {
	addAttribute(fingerCount, attributeCounter, result, datasetFile);
}

void FingerDiff::nearestFingersDistancesAttribute(GestureHand* tempHand, int fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile) {
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
			addAttribute(distance, attributeCounter, result, datasetFile);
			firstBaseFingerPosition = baseFingerPosition;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::nearestFingersDistancesRatiosAttribute(GestureHand* tempHand, int fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile) {
	//distance between two nearest base points of a finger
	if (tempHand != NULL && fingerCount > 1) {
		float minimalDistance = 0;
		std::vector<double> distances;
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
		for (uint i = 0; i < distances.size(); i++) {
			float distanceRatio = 0;
			if (minimalDistance > 0)
				distanceRatio = distances[i] / minimalDistance;

			addAttribute(distanceRatio, attributeCounter, result, datasetFile);
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::fingerThicknessRatiosAttribute(int& fingerCount, GestureHand* tempHand,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile) {
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
				datasetFile);
	}
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile) {
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
			addAttribute(angle, attributeCounter, result, datasetFile);
			leftFingerDirection = rightFingerDirection;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::anglesBetweenFingersRelativeToPalmPosAttribute(GestureHand* tempHand,
		int fingerCount, int attributeCounter, std::vector<double>& result,
		FileWriterUtil& datasetFile) {
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
			addAttribute(angle, attributeCounter, result, datasetFile);
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount); i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::addAttribute(float attributeValue, int& attributeCounter,
		std::vector<double> &attributes, FileWriterUtil& datasetFile) {
	attributes.push_back(attributeValue);
	datasetFile<<attributeCounter<<":"<<attributeValue<<" ";
	attributeCounter++;
}
