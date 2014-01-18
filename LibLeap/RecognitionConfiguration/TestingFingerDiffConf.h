#include <string>

#ifndef TESTINGFINGERDIFFCONF_H_
#define TESTINGFINGERDIFFCONF_H_

class TestingFingerDiffConf {
public:
	const static std::string DEFAULT_CONF_NAME;

	std::string configurationPath;
	std::string configurationName;
	double classificationThresholdRate;

	TestingFingerDiffConf();
};

#endif /* TESTINGFINGERDIFFCONF_H_ */
