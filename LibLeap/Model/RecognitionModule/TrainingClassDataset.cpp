#include "TrainingClassDataset.h"

TrainingClassDataset::TrainingClassDataset(std::string className, std::vector<GestureFrame> dataset)
{
	this->className = className;
	this->dataset = dataset;
}

