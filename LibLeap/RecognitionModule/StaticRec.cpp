#include "StaticRec.h"

StaticRec::StaticRec() {
	logger = new LogUtil("Finger differentiation");
}

StaticRec::~StaticRec() {
	delete logger;
}

void StaticRec::setTrainingConfiguration(TrainingStaticRecConf configuration) {
	this->confPath = configuration.configurationPath;
	this->confName = configuration.configurationName;
	this->saveDatasetFile = configuration.saveDatasetFile;
	this->saveScaleFile = configuration.saveScaleFile;
	this->featureVersion = configuration.featureSetVersion;
}

void StaticRec::setTestingConfiguration(TestingStaticRecConf configuration) {
	this->confPath = configuration.configurationPath;;
	this->confName = configuration.configurationName;
	this->featureVersion = configuration.featureSetVersion;
}

std::vector<double> StaticRec::addFeatures(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile) {
	// Adding the finger count to the feature set
	fingerCountAttribute(fingerCount, attributeCounter, result, datasetFile);

	// Versions:
	// 1-> LP, dist 5 fin, 5 angles
	// 2-> LP, dist 5 fin, 5 angles, 5 dist to palms,
	// 3-> LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
	// sorted ones:
	// 4-> LP, dist 5 fin, 5 angles
	// 5-> LP, dist 5 fin, 5 angles, 5 dist to palms,
	// 6-> LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
	int sorted = 0;
	if (featureVersion > 3) sorted = 1;

	// Adding the 5 highest angles to the feature set
	anglesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, datasetFile, sorted);

	// Adding the 5 greatest distances to the feature set
	distancesBetweenFingersAttribute(tempHand, fingerCount, attributeCounter,
			result, datasetFile);

	if ((featureVersion - 1) % 3 >= 1) {
		distancesFingersPalmAttribute(tempHand, fingerCount, attributeCounter,
				result, datasetFile, sorted);

		if ((featureVersion - 1) % 3 > 1) {
			// Adding 5 angles to the palm normal
			anglesFingersPalmAttribute(tempHand, fingerCount, attributeCounter,
					result, datasetFile, sorted);
		}
	}

	return result;
}

void StaticRec::fingerCountAttribute(int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile) {
	addAttribute(fingerCount, attributeCounter, result, datasetFile);
}

void StaticRec::anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {


		std::vector<float> angles(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex fingerDirection = tempHand->getFinger(i)->getDirection();

			int nextOrAll = std::min(i+1, fingerCount);
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
			addAttribute(angles[i], attributeCounter, result, datasetFile);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result, datasetFile);
		}
	}
}

void StaticRec::anglesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		std::vector<float> angles(5, 0.0);

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
			addAttribute(angles[i], attributeCounter, result, datasetFile);
		}
	}
	// There are no fingers in the captured data
	else {
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result, datasetFile);
		}
	}
}

void StaticRec::distancesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		std::vector<float> distances(5, 0.0);

		// For all combinations of finger configurations
		for (int i = 0; i < fingerCount; i++) {
			Vertex tipPosition = tempHand->getFinger(i)->getStabilizedTipPosition();

			int nextOrAll = std::min(i + 1, fingerCount);
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
			addAttribute(distances[i], attributeCounter, result, datasetFile);
		}
	}
	// There are no fingers in the captured data
	else
	{
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result, datasetFile);
		}
	}
}

void StaticRec::distancesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
		int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted) {

	// If Hand exists and we have fingers
	if (tempHand != NULL && fingerCount > 1) {

		std::vector<float> distances(5, 0.0);

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
			addAttribute(distances[i], attributeCounter, result, datasetFile);
		}
	}
	// There are no fingers in the captured data
	else {
		for (int i = 0; i < 5; i++) {
			addAttribute(0.0, attributeCounter, result, datasetFile);
		}
	}
}

