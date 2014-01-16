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

class SVMclassificator {
public:
	void train(std::vector<std::vector<double> >& trainDataset,
			std::vector<int>& trainLabels, int classNumber,
			std::string configurationPath, std::string configurationName);
	std::vector<int> classify(std::vector<std::vector<double> > data);

private:
	int classNumber;

	svm_problem *svmTrainingSet;
	svm_model *svmModel;

	int featureSize;
	double **scaling;
	void dataScaling(std::vector<std::vector<double> >& data);
	void dataScalingClassify(std::vector<std::vector<double> >& data);

	svm_problem* defineSvmTrainingSet(std::vector<std::vector<double> >& trainDataset,
			std::vector<int>& trainLabels);
	svm_parameter* setTrainingParameters();
};

#endif /* SVMCLASSIFICATOR_H_ */
