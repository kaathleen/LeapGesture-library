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

	TrainingFingerDiffConf(std::string configurationPath = "", std::string configurationName = DEFAULT_CONF_NAME, bool saveDatasetFile = false, int kCrossValParam = 5);
};

#endif /* TRAININGFINGERDIFFCONF_H_ */
