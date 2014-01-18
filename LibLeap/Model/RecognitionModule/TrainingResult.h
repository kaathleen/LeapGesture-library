#include <vector>

#include "TrainingClassResult.h"

#ifndef TRAININGRESULT_H_
#define TRAININGRESULT_H_

class TrainingResult {
public:
	double trainRate;
	std::vector <TrainingClassResult> trainClassResults;

	TrainingResult(double trainRate, std::vector <TrainingClassResult> trainClassResults);

	TrainingResult();
};

#endif /* TRAININGRESULT_H_ */
