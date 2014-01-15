#include "ClassDataset.h"

ClassDataset::ClassDataset(std::string className, std::vector<GestureFrame> dataset)
{
	this->className = className;
	this->dataset = dataset;
}

