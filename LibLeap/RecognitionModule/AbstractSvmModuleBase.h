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

#include "../Model/RecognitionModule/TrainingClassDataset.h"
#include "../Model/RecognitionModule/TrainingResult.h"

#include "../Model/RecognitionModule/TestingFrame.h"
#include "../Model/RecognitionModule/TestingResult.h"

#include "../RecognitionConfiguration/TestingStaticRecConf.h"
#include "../RecognitionConfiguration/TrainingStaticRecConf.h"

#ifndef ABSTRACTSVMMODULEBASE_H_
#define ABSTRACTSVMMODULEBASE_H_

class AbstractSvmModuleBase {
public:
	AbstractSvmModuleBase();
	virtual ~AbstractSvmModuleBase() = 0;
protected:
	LogUtil* logger;

	const static int MAX_FINGER_COUNT;

	std::string confPath;
	std::string confName;
	bool saveDatasetFile;
	bool saveScaleFile;

	std::vector<std::string> genericClassNames;

	virtual void createTrainingFeaturesDataSet(TrainingClassDatasetList& classDatasets,
			std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels, bool saveDatasetFile);
	virtual void createTestingFeaturesDataSet(TestingFrame &testingFrame, std::vector<double>& testDataset);

	virtual void createGenericClassNames(TrainingClassDatasetList& classDatasets);
	virtual void saveGenericClassNames();
	virtual void loadGenericClassNames(std::string filePath);

	virtual std::vector<double> createFeatureSet(GestureFrame *gestureFrame, FileWriterUtil* datasetFile = NULL);
	virtual std::vector<double> addFeatures(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile) = 0;

	// feature util
	virtual void addAttribute(float attributeValue, int& attributeCounter, std::vector<double> &attributes, FileWriterUtil* datasetFile);
private:
};

#endif /* ABSTRACTSVMMODULE_H_ */
