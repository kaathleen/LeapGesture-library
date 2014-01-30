#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "../Types/ConfExt.h"
#include "../Util/PathUtil.h"
#include "../Util/FileWriterUtil.h"
#include "../Util/LogUtil.h"

#include "SVMclassificator.h"
#include "AbstractSvmModuleBase.h"

#include "../Model/RecognitionModule/TrainingClassDataset.h"
#include "../Model/RecognitionModule/TrainingResult.h"

#include "../Model/RecognitionModule/TestingFrame.h"
#include "../Model/RecognitionModule/TestingResult.h"

#include "../RecognitionConfiguration/TestingStaticRecConf.h"
#include "../RecognitionConfiguration/TrainingStaticRecConf.h"

#ifndef ABSTRACTSVMMODULE_H_
#define ABSTRACTSVMMODULE_H_

template <class TRAIN_C, class TEST_C>
class AbstractSvmModule : public AbstractSvmModuleBase {
public:
	virtual TrainingResult* train(TrainingClassDatasetList &classDatasetList, TRAIN_C configuration) {
		logger->debug("train entry");

		setTrainingConfiguration(configuration);

		createGenericClassNames(classDatasetList);
		saveGenericClassNames();

		std::vector<std::vector<double> > trainDataset; //features in samples
		std::vector<int> trainLabels;
		createTrainingFeaturesDataSet(classDatasetList, trainDataset, trainLabels,
				saveDatasetFile);

		SVMclassificator svm;
		TrainingResult *trainResult = svm.train(trainDataset, trainLabels,
				genericClassNames.size(), confPath, confName,
				saveScaleFile, configuration.kCrossValParam);

		for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
			trainResult->trainClassResults[i].className = genericClassNames[i];
		}

		logger->debug("train exit");
		return trainResult;
	}

	virtual TestingResult* classify(TestingFrame &testingFrame, TEST_C configuration) {
		logger->debug("classify entry");

		setTestingConfiguration(configuration);

		std::string genericClassFilePath = PathUtil::combinePathFileNameAndExt(
				confPath, confName, ConfExt::CLASS_MAP_EXT);
		loadGenericClassNames(genericClassFilePath);

		std::vector<double> testDataset; //features in samples
		createTestingFeaturesDataSet(testingFrame, testDataset);

		SVMclassificator svm;
		TestingResult *testResult = svm.classify(testDataset,
				genericClassNames.size(), confPath, confName,
				configuration.classificationThresholdRate);

		if (testResult->recognized) {
			testResult->className = genericClassNames[testResult->genericClassName];
			testResult->frameTimestamp = testingFrame.frame.getTimestamp();
			for (unsigned int i = 0;
					i < testResult->testClassResults.size(); i++) {
				int genericClassIndex =
						testResult->testClassResults[i].genericClassName;
				testResult->testClassResults[i].className =
						genericClassNames[genericClassIndex];
			}
		}

		logger->debug("classify exit");
		return testResult;
	}
protected:
	virtual void setTrainingConfiguration(TRAIN_C configuration) = 0;
	virtual void setTestingConfiguration(TEST_C configuration) = 0;
private:
};

#endif /* ABSTRACTSVMMODULE_H_ */
