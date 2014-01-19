#include <string>

#ifndef TRAININGFINGERDIFFCONF_H_
#define TRAININGFINGERDIFFCONF_H_

class TrainingFingerDiffConf {
public:
	const static std::string DEFAULT_CONF_NAME;

	std::string configurationPath;
	std::string configurationName;
	bool saveDatasetFile;
	int kCrossValParam;

	TrainingFingerDiffConf();
};

#endif /* TRAININGFINGERDIFFCONF_H_ */
