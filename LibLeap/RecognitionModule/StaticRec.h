#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "../Types/ConfExt.h"
#include "../Types/StaticRecFeatureVersion.h"
#include "../Util/PathUtil.h"
#include "../Util/FileWriterUtil.h"
#include "../Util/LogUtil.h"

#include "AbstractSvmModule.h"
#include "SVMclassificator.h"

#include "../Model/RecognitionModule/TrainingClassDataset.h"
#include "../Model/RecognitionModule/TrainingResult.h"

#include "../Model/RecognitionModule/TestingFrame.h"
#include "../Model/RecognitionModule/TestingResult.h"

#include "../RecognitionConfiguration/TestingStaticRecConf.h"
#include "../RecognitionConfiguration/TrainingStaticRecConf.h"

#ifndef STATICREC_H_
#define STATICREC_H_

class StaticRec : public AbstractSvmModule<TrainingStaticRecConf, TestingStaticRecConf>{
public:
	StaticRec();
	~StaticRec();
private:
	StaticRecFeatureVersion featureVersion;

	void setTrainingConfiguration(TrainingStaticRecConf configuration);
	void setTestingConfiguration(TestingStaticRecConf configuration);

	std::vector<double> addFeatures(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);

	// features
	void fingerCountAttribute(int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
			int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted = 1);
	void anglesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
			int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted = 1);
	void distancesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount,
			int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted = 1);
	void distancesFingersPalmAttribute(GestureHand* tempHand, int& fingerCount,
			int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile, int sorted = 1);
};

#endif /* STATICREC_H_ */
