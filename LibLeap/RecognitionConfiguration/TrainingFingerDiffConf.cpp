#include "TrainingFingerDiffConf.h"

const std::string TrainingFingerDiffConf::DEFAULT_CONF_NAME = "defaultDataset";

TrainingFingerDiffConf::TrainingFingerDiffConf() {
	this->configurationName = DEFAULT_CONF_NAME;
	this->saveDatasetFile = false;
	this->kCrossValParam = 5;
}
