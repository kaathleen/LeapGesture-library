#include <string>

#ifndef TRAININGCLASSRESULT_H_
#define TRAININGCLASSRESULT_H_

class TrainingClassResult {
public:
	double classTrainRate;
	std::string className;
	int genericClassName;

	TrainingClassResult(int genericClassName, double classTrainRate, std::string className = "");
};

#endif /* TRAININGCLASSRESULT_H_ */
