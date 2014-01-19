#include "FingerDiff.h"

FingerDiff::FingerDiff() {
	logger = new LogUtil("Finger differentiation");
}

FingerDiff::~FingerDiff() {
	delete logger;
}

TrainingResult* FingerDiff::train(TrainingClassDatasetList& classDatasetList,
		TrainingFingerDiffConf configuration) {
	logger->debug("train entry");

	this->confPath = configuration.configurationPath;
	this->confName = configuration.configurationName;

	createGenericClassNames(classDatasetList);
	saveGenericClassNames();

	std::vector<std::vector<double> > trainDataset; //features in samples
	std::vector<int> trainLabels;
	createTrainingFeaturesDataSet(classDatasetList, trainDataset, trainLabels,
			configuration.saveDatasetFile);

	SVMclassificator svm;
	TrainingResult *trainResult = svm.train(trainDataset, trainLabels,
			genericClassNames.size(), confPath, confName,
			configuration.saveDatasetFile, configuration.kCrossValParam);

	for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
		trainResult->trainClassResults[i].className = genericClassNames[i];
	}

	logger->debug("train exit");
	return trainResult;
}

TestingResult* FingerDiff::classify(TestingFrame &testingFrame, TestingFingerDiffConf configuration) {
	logger->debug("classify entry");

	this->confPath = configuration.configurationPath;
	this->confName = configuration.configurationName;
	std::string genericClassFilePath = PathUtil::combinePathFileNameAndExt(
			confPath, confName, ConfExt::CLASS_MAP_EXT);
	loadGenericClassNames(genericClassFilePath);

	std::vector<double> testDataset; //features in samples
	createTestingFeaturesDataSet(testingFrame, testDataset);

	SVMclassificator svm;
	TestingResult *testResult = svm.classify(testDataset,
			genericClassNames.size(), confPath, confName,
			configuration.classificationThresholdRate);

	if (testResult->recognized) {
		testResult->className = genericClassNames[testResult->genericClassName];
		testResult->frameTimestamp = testingFrame.frame.getTimestamp();
		for (unsigned int i = 0;
				i < testResult->classificationClassResults.size(); i++) {
			int genericClassIndex =
					testResult->classificationClassResults[i].genericClassName;
			testResult->classificationClassResults[i].className =
					genericClassNames[genericClassIndex];
		}
	}

	logger->debug("classify exit");
	return testResult;
}

void FingerDiff::createTrainingFeaturesDataSet(
		TrainingClassDatasetList& classDatasetList,
		std::vector<std::vector<double> >& trainDataset,
		std::vector<int>& trainLabels, bool saveDatasetFile) {
	std::string datasetFilePath = PathUtil::combinePathFileNameAndExt(
			confPath, confName, ConfExt::DAT_EXT);
	FileWriterUtil datasetFile(datasetFilePath, !saveDatasetFile);
	datasetFile.open();

	for (unsigned int i = 0; i < classDatasetList.size(); i++) {
		for (unsigned int j = 0; j < classDatasetList[i].dataset.size(); j++) {
			// Defining the set of features
			std::vector<double> row;
			// sequence number of class
			datasetFile << classDatasetList[i].genericClassName << " ";
			row = createFeatureSet(&(classDatasetList[i].dataset[j]),
					&datasetFile);
			datasetFile << "\n";

			trainDataset.push_back(row);
			trainLabels.push_back(classDatasetList[i].genericClassName);
		}
	}

	datasetFile.close();
}

void FingerDiff::createTestingFeaturesDataSet(TestingFrame &testingFrame,
		std::vector<double>& testDataset) {
	// sequence number of class
	testDataset = createFeatureSet(&(testingFrame.frame));
}

std::vector<double> FingerDiff::addFeatures(GestureHand* tempHand,
		int fingerCount, int& attributeCounter, std::vector<double>& result,
		FileWriterUtil* datasetFile) {
	//fingerCount
	fingerCountAttribute(fingerCount, attributeCounter, result, datasetFile);

	//distance between two nearest base points of a finger
	/*nearestFingersDistancesAttribute(tempHand, fingerCount, attributeCounter,
	 result, svmGesture);*/

	//ratio of distance between two nearest base points of a finger to the minimal (non-zero) distance between two nearest base points
	nearestFingersDistancesRatiosAttribute(tempHand, fingerCount,
			attributeCounter, result, datasetFile);

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
		std::vector<double>& result, FileWriterUtil* datasetFile) {
	addAttribute(fingerCount, attributeCounter, result, datasetFile);
}

void FingerDiff::nearestFingersDistancesAttribute(GestureHand* tempHand,
		int fingerCount, int& attributeCounter, std::vector<double>& result,
		FileWriterUtil* datasetFile) {
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
					abs(
							(firstBaseFingerPosition - baseFingerPosition).getMagnitude());
			addAttribute(distance, attributeCounter, result, datasetFile);
			firstBaseFingerPosition = baseFingerPosition;
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (AbstractSvmModule::MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::nearestFingersDistancesRatiosAttribute(GestureHand* tempHand,
		int fingerCount, int& attributeCounter, std::vector<double>& result,
		FileWriterUtil* datasetFile) {
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
					abs(
							(firstBaseFingerPosition - baseFingerPosition).getMagnitude());
			if ((minimalDistance == 0 || distance < minimalDistance)
					&& distance > 0) {
				minimalDistance = distance;
			}

			distances.push_back(distance);
			firstBaseFingerPosition = baseFingerPosition;
		}
		for (unsigned int i = 0; i < distances.size(); i++) {
			float distanceRatio = 0;
			if (minimalDistance > 0)
				distanceRatio = distances[i] / minimalDistance;

			addAttribute(distanceRatio, attributeCounter, result, datasetFile);
		}
	}
	fingerCount = (fingerCount == 0) ? 1 : fingerCount;
	for (int i = 0; i < (AbstractSvmModule::MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::fingerThicknessRatiosAttribute(int& fingerCount,
		GestureHand* tempHand, int& attributeCounter,
		std::vector<double>& result, FileWriterUtil* datasetFile) {
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
	for (int i = 0; i < (AbstractSvmModule::MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::anglesBetweenFingersAttribute(GestureHand* tempHand,
		int& fingerCount, int& attributeCounter, std::vector<double>& result,
		FileWriterUtil* datasetFile) {
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
	for (int i = 0; i < (AbstractSvmModule::MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}

void FingerDiff::anglesBetweenFingersRelativeToPalmPosAttribute(
		GestureHand* tempHand, int fingerCount, int attributeCounter,
		std::vector<double>& result, FileWriterUtil* datasetFile) {
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
	for (int i = 0; i < (AbstractSvmModule::MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}
