#include <string>

#include "../GestureFrame.h"

#ifndef CLASSDATASET_H_
#define CLASSDATASET_H_

class ClassDataset {

public:
	std::string className;
	std::vector<GestureFrame> dataset;
	int genericClassName;

	ClassDataset(std::string className, std::vector<GestureFrame> dataset);
};

typedef std::vector<ClassDataset> ClassDatasets;

#endif /* CLASSDATASET_H_ */
