#include <string>

#ifndef TRAININGFINGERDIFFCONF_H_
#define TRAININGFINGERDIFFCONF_H_

class TrainingFingerDiffConf {
public:
	std::string configurationPath;
	std::string configurationName;
	bool saveDatasetFile;
	bool saveScaleFile;
	int kCrossValParam;

	TrainingFingerDiffConf(std::string configurationPath,
			std::string configurationName,
			bool saveDatasetFile = false, bool saveScaleFile = false, int kCrossValParam = 5);
};

#endif /* TRAININGFINGERDIFFCONF_H_ */
