/*
 * SVMclassificator.h
 *
 *  Created on: 22-11-2013
 *      Author: smi
 */

#ifndef SVMCLASSIFICATOR_H_
#define SVMCLASSIFICATOR_H_

#include <vector>
#include <svm.h>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

class SVMclassificator
{
public:
	svm_problem *svmTrainingSet;
	svm_model *svmModel;

	int NUMBER_OF_CLASSES;

	void train(std::vector< std::vector<double> > data, std::vector<int> label, int NUMBER_OF_CLASSES);
	std::vector<int> classify (std::vector< std::vector< double> > data);

private:
	int featureSize;
	double **scaling;
	void dataScaling(std::vector<std::vector<double> >& data);
	void dataScalingClassify(std::vector<std::vector<double> >& data);
};


#endif /* SVMCLASSIFICATOR_H_ */
