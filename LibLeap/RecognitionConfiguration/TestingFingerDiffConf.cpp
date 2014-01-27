#include "TestingFingerDiffConf.h"

const std::string TestingFingerDiffConf::DEFAULT_CONF_NAME = "defaultDataset";

TestingFingerDiffConf::TestingFingerDiffConf(std::string configurationPath, std::string configurationName, double classificationThresholdRate) {
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->classificationThresholdRate = classificationThresholdRate;
}

