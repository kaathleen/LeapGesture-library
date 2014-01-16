#include <vector>
#include <string>

#ifndef TRAINRESULT_H_
#define TRAINRESULT_H_

class TrainClassResult {
public:
	TrainClassResult(std::string className, int genericClassName, double trainRate);

	const std::string getClassName() {
		return className;
	}
	const int getGenericClassName() {
		return genericClassName;
	}
	const double getTrainRate() {
		return trainRate;
	}
private:
	std::string className;
	int genericClassName;
	double trainRate;
};

typedef std::vector<TrainClassResult> TrainResult;

#endif /* TRAINRESULT_H_ */
