#include "TestingStaticRecConf.h"

TestingStaticRecConf::TestingStaticRecConf(std::string configurationPath,
		std::string configurationName,
		StaticRecFeatureVersion featureSetVersion,
		double classificationThresholdRate) {
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->featureSetVersion = featureSetVersion;
	this->classificationThresholdRate = classificationThresholdRate;
}

