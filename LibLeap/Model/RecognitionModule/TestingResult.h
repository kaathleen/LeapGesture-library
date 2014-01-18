#include <vector>

#include "TestingClassResult.h"

#ifndef TESTINGRESULT_H_
#define TESTINGRESULT_H_

class TestingResult {
public:
	bool recognized;
	std::string className;
	int genericClassName;
	std::vector<TestingClassResult> classificationClassResults;

	double frameTimestamp;

	TestingResult();
};

#endif /* TESTINGRESULT_H_ */
