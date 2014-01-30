#include "TrainingFingerDiffConf.h"

TrainingFingerDiffConf::TrainingFingerDiffConf(std::string configurationPath,
		std::string configurationName, bool saveDatasetFile, bool saveScaleFile,
		int kCrossValParam) {
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->saveDatasetFile = saveDatasetFile;
	this->saveScaleFile = saveScaleFile;
	this->kCrossValParam = kCrossValParam;
}
