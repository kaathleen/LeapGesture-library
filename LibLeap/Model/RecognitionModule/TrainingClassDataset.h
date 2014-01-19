#include <string>

#include "../GestureFrame.h"

#ifndef TRAININGCLASSDATASET_H_
#define TRAININGCLASSDATASET_H_

class TrainingClassDataset {

public:
	std::string className;
	std::vector<GestureFrame> dataset;
	int genericClassName;

	TrainingClassDataset(std::string className, std::vector<GestureFrame> dataset);
};

typedef std::vector<TrainingClassDataset> TrainingClassDatasetList;

#endif /* TRAININGCLASSDATASET_H_ */
