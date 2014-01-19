#include <vector>
#include <svm.h>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Types/ConfExt.h"
#include "../Util/PathUtil.h"
#include "../Util/FileWriterUtil.h"
#include "../Util/LogUtil.h"
#include "../Model/RecognitionModule/TrainingResult.h"
#include "../Model/RecognitionModule/TestingResult.h"

#ifndef SVMCLASSIFICATOR_H_
#define SVMCLASSIFICATOR_H_

class SVMclassificator {
public:
	SVMclassificator();
	~SVMclassificator();

	TrainingResult* train(std::vector<std::vector<double> >& trainDataset,
			std::vector<int>& trainLabels, int classNumber,
			std::string configurationPath, std::string configurationName,
			bool saveScaleFile = false, int kCrossValParam = 5);
	TestingResult* classify(std::vector<double>& testDataset, int classNumber,
			std::string configurationPath, std::string configurationName, double classificationThresholdRate=0.5);

private:
	struct FeatureRange {
		int featureIndex;
		double a, b;
	};

	LogUtil* logger;

	const static int TRAIN_LOWER_RANGE;
	const static int TRAIN_UPPER_RANGE;
	int testLowerRange;
	int testUpperRange;

	std::string trainConfPath;
	std::string trainConfName;
	std::string testConfPath;
	std::string testConfName;
	bool saveScaleFile;

	svm_problem *svmTrainingSet;

	void dataScalingTrain(std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels);
	void dataScalingClassify(std::vector<double>& testDataset);
	double scaleValue(double value, double minValue, double maxValue, int lowerRange, int upperRange);

	svm_problem* defineSvmTrainingSet(std::vector<std::vector<double> >& trainDataset,
			std::vector<int>& trainLabels);
	svm_parameter* setTrainingParameters();
	void searchLearningParameters(svm_parameter*& svmParameter, std::vector<int>& trainLabels, int classNumber, int kCrossValParam, double best_cross, double& best_g, double& best_c, TrainingResult* trainResult);

	void saveScaleAndRangeFiles(std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels, double** scaling, bool* resetFeatures);
	double** loadRangeFile(int featureSize);
};

#endif /* SVMCLASSIFICATOR_H_ */
