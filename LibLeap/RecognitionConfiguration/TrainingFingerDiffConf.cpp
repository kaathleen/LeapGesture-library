#include "TrainingFingerDiffConf.h"

const std::string TrainingFingerDiffConf::DEFAULT_CONF_NAME = "defaultDataset";

TrainingFingerDiffConf::TrainingFingerDiffConf(std::string configurationPath, std::string configurationName, bool saveDatasetFile, int kCrossValParam) {
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->saveDatasetFile = saveDatasetFile;
	this->kCrossValParam = kCrossValParam;
}
