#include "AbstractSvmModuleBase.h"

const int AbstractSvmModuleBase::MAX_FINGER_COUNT = 5;

AbstractSvmModuleBase::AbstractSvmModuleBase() {
	saveDatasetFile = false;
	saveScaleFile = false;
}

AbstractSvmModuleBase::~AbstractSvmModuleBase() {}

void AbstractSvmModuleBase::createTrainingFeaturesDataSet(TrainingClassDatasetList& classDatasetList,
		std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels, bool saveDatasetFile) {
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
			row = createFeatureSet(&(classDatasetList[i].dataset[j]), &datasetFile);
			datasetFile << "\n";

			trainDataset.push_back(row);
			trainLabels.push_back(classDatasetList[i].genericClassName);
		}
	}

	datasetFile.close();
}

void AbstractSvmModuleBase::createTestingFeaturesDataSet(TestingFrame &testingFrame, std::vector<double>& testDataset) {
	// sequence number of class
	testDataset = createFeatureSet(&(testingFrame.frame));
}

std::vector<double> AbstractSvmModuleBase::createFeatureSet(GestureFrame *gestureFrame,
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

void AbstractSvmModuleBase::createGenericClassNames(
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

void AbstractSvmModuleBase::saveGenericClassNames() {
	std::string genericClassFilePath = PathUtil::combinePathFileNameAndExt(
			confPath, confName, ConfExt::CLASS_MAP_EXT);
	FileWriterUtil genericClassFile(genericClassFilePath);
	genericClassFile.open();
	for (unsigned int i = 0; i < genericClassNames.size(); i++) {
		genericClassFile<< genericClassNames[i] << "\n";
	}
	genericClassFile.close();
}

void AbstractSvmModuleBase::loadGenericClassNames(std::string filePath) {
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

void AbstractSvmModuleBase::addAttribute(float attributeValue, int& attributeCounter,
		std::vector<double> &attributes, FileWriterUtil* datasetFile) {
	attributes.push_back(attributeValue);
	if (datasetFile != NULL) {
		(*datasetFile) << attributeCounter << ":" << attributeValue << " ";
	}
	attributeCounter++;
}
