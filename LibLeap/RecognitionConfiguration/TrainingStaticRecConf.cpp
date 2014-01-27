#include "TrainingStaticRecConf.h"

TrainingStaticRecConf::TrainingStaticRecConf(std::string configurationPath,
		std::string configurationName, bool saveDatasetFile,
		StaticRecFeatureVersion featureSetVersion, int kCrossValParam) {
	this->configurationPath = configurationPath;
	this->configurationName = configurationName;
	this->saveDatasetFile = saveDatasetFile;
	this->featureSetVersion = featureSetVersion;
	this->kCrossValParam = kCrossValParam;
}

