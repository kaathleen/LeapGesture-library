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

#include "AbstractSvmModule.h"
#include "SVMclassificator.h"

#include "../Model/RecognitionModule/TrainingClassDataset.h"
#include "../Model/RecognitionModule/TrainingResult.h"

#include "../Model/RecognitionModule/TestingFrame.h"
#include "../Model/RecognitionModule/TestingResult.h"

#include "../RecognitionConfiguration/TestingFingerDiffConf.h"
#include "../RecognitionConfiguration/TrainingFingerDiffConf.h"

#ifndef FINGERDIFF_H_
#define FINGERDIFF_H_

class FingerDiff : public AbstractSvmModule<TrainingFingerDiffConf, TestingFingerDiffConf> {
public:
	FingerDiff();
	~FingerDiff();

private:
	void setTrainingConfiguration(TrainingFingerDiffConf configuration);
	void setTestingConfiguration(TestingFingerDiffConf configuration);

	std::vector<double> addFeatures(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);

	// features
	void fingerCountAttribute(int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void nearestFingersDistancesAttribute(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void nearestFingersDistancesRatiosAttribute(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void fingerThicknessRatiosAttribute(int& fingerCount, GestureHand* tempHand, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
	void anglesBetweenFingersRelativeToPalmPosAttribute(GestureHand* tempHand, int fingerCount, int attributeCounter, std::vector<double>& result, FileWriterUtil* datasetFile);
};

#endif /* FINGERDIFF_H_ */
