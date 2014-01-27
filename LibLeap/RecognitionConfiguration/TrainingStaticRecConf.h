#include <string>

#include "../Types/StaticRecFeatureVersion.h"

#ifndef TRAININGSTATICRECCONF_H_
#define TRAININGSTATICRECCONF_H_

class TrainingStaticRecConf {
public:
	std::string configurationPath;
	std::string configurationName;
	bool saveDatasetFile;
	StaticRecFeatureVersion featureSetVersion;
	int kCrossValParam;

	TrainingStaticRecConf(std::string configurationPath,
			std::string configurationName, bool saveDatasetFile = false,
			StaticRecFeatureVersion featureSetVersion = VERSION6, int kCrossValParam = 5);
};

#endif /* TRAININGSTATICRECCONF_H_ */
