#include "FingerDiff.h"

FingerDiff::FingerDiff() {
	logger = new LogUtil("Finger differentiation");
}

FingerDiff::~FingerDiff() {
	delete logger;
}

void FingerDiff::setTrainingConfiguration(TrainingFingerDiffConf configuration) {
	this->confPath = configuration.configurationPath;
	this->confName = configuration.configurationName;
	this->saveDatasetFile = configuration.saveDatasetFile;
	this->saveScaleFile = configuration.saveScaleFile;
}

void FingerDiff::setTestingConfiguration(TestingFingerDiffConf configuration) {
	this->confPath = configuration.configurationPath;;
	this->confName = configuration.configurationName;
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
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount);
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
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount);
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
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount);
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
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount);
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
	for (int i = 0; i < (MAX_FINGER_COUNT - fingerCount);
			i++) {
		addAttribute(0, attributeCounter, result, datasetFile);
	}
}
