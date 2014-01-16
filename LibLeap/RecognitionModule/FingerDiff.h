#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "../Types/ConfExt.h"
#include "../Util/PathUtil.h"
#include "../Util/FileWriterUtil.h"
#include "../Model/RecognitionModule/ClassDataset.h"
#include "../Model/RecognitionModule/TrainClassResult.h"
#include "SVMclassificator.h"

#ifndef FINGERDIFF_H_
#define FINGERDIFF_H_

class FingerDiff {

public:
	FingerDiff();
	TrainResult train(ClassDatasets &classDatasets, std::string configurationPath, std::string configurationName, bool saveDatasetFile = false);

private:
	const static int MAX_FINGER_COUNT;

	std::string configurationPath;
	std::string configurationName;
	bool saveDatasetFile;

	std::vector<std::string> genericClassNames;

	void createGenericClassNames(std::vector<ClassDataset>& classDatasets);
	void saveGenericClassNames();
	void createFeaturesDataSet(std::vector<ClassDataset>& classDatasets,
			std::vector<std::vector<double> >& trainDataset, std::vector<int>& trainLabels);
	std::vector<double> createFeatures(GestureFrame *gestureFrame, FileWriterUtil& datasetFile);

	// features
	void fingerCountAttribute(int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);
	void nearestFingersDistancesAttribute(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);
	void nearestFingersDistancesRatiosAttribute(GestureHand* tempHand, int fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);
	void fingerThicknessRatiosAttribute(int& fingerCount, GestureHand* tempHand, int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);
	void anglesBetweenFingersAttribute(GestureHand* tempHand, int& fingerCount, int& attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);
	void anglesBetweenFingersRelativeToPalmPosAttribute(GestureHand* tempHand, int fingerCount, int attributeCounter, std::vector<double>& result, FileWriterUtil& datasetFile);

	// feature utils
	void addAttribute(float attributeValue, int& attributeCounter, std::vector<double> &attributes, FileWriterUtil& datasetFile);
};

#endif /* FINGERDIFF_H_ */
