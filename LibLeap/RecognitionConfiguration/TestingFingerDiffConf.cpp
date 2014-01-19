#include "TestingFingerDiffConf.h"

const std::string TestingFingerDiffConf::DEFAULT_CONF_NAME = "defaultDataset";

TestingFingerDiffConf::TestingFingerDiffConf() {
	this->configurationName = DEFAULT_CONF_NAME;
	this->classificationThresholdRate = 0.5;
}

