#include "AbstractSvmModule.h"

const int AbstractSvmModule::MAX_FINGER_COUNT = 5;

AbstractSvmModule::AbstractSvmModule() {}

AbstractSvmModule::~AbstractSvmModule() {}

std::vector<double> AbstractSvmModule::createFeatureSet(GestureFrame *gestureFrame,
		FileWriterUtil* datasetFile) {
	std::vector<double> result;
	GestureHand *tempHand = gestureFrame->getHand(0);
	int fingerCount =
			(tempHand != NULL) ?
					std::min(tempHand->getFingerCount(), MAX_FINGER_COUNT) : 0;
	int attributeCounter = 1;

	addFeatures(tempHand, fingerCount, attributeCounter, result, datasetFile);

	return result;
}

void AbstractSvmModule::createGenericClassNames(
		TrainingClassDatasetList& classDatasets) {
	for (unsigned int i = 0; i < classDatasets.size(); i++) {
		std::string className = classDatasets[i].className;
		bool isNewClass = true;
		unsigned int j;
		for (j = 0; j < genericClassNames.size(); j++) {
			if (genericClassNames[j] == className) {
				isNewClass = false;
				break;
			}
		}

		if (isNewClass) {
			genericClassNames.push_back(className);
			classDatasets[i].genericClassName = genericClassNames.size() - 1;
		} else {
			classDatasets[i].genericClassName = j;
		}
	}
}

void AbstractSvmModule::saveGenericClassNames() {
	std::string genericClassFilePath = PathUtil::combinePathFileNameAndExt(
			confPath, confName, ConfExt::CLASS_MAP_EXT);
	FileWriterUtil genericClassFile(genericClassFilePath);
	genericClassFile.open();
	for (unsigned int i = 0; i < genericClassNames.size(); i++) {
		genericClassFile<< genericClassNames[i] << "\n";
	}
	genericClassFile.close();
}

void AbstractSvmModule::loadGenericClassNames(std::string filePath) {
	std::ifstream genericClassFile;
	genericClassFile.open(filePath.c_str(), std::fstream::in);

	if (!genericClassFile.is_open()) {
		// throw exception
	}

	std::string line;
	genericClassNames.clear();
	while (std::getline(genericClassFile, line)) {
		genericClassNames.push_back(line);
	}

	genericClassFile.close();
}

void AbstractSvmModule::addAttribute(float attributeValue, int& attributeCounter,
		std::vector<double> &attributes, FileWriterUtil* datasetFile) {
	attributes.push_back(attributeValue);
	if (datasetFile != NULL) {
		(*datasetFile) << attributeCounter << ":" << attributeValue << " ";
	}
	attributeCounter++;
}
