/*
 * Learning.cpp
 *
 *  Created on: 29-01-2014
 *      Author: oli
 */

#include "Learning.h"
#include "RecognizedGesture.h"

using namespace std;

Learning::Learning() {
	// TODO Auto-generated constructor stub

}


void Learning::learnStatic(vector<pair<const char*, const char*> > files, char* path, char* filename) {

	cout << "START STATIC REC TEST";

	StaticRec staticRec;

	LogUtil::setQuietMode(false);

	GestureStorageDriver *gsd = new BinaryFileStorageDriver();
	std::vector<TrainingClassDataset> classDatasets;


	for(int i=0;i<files.size(); i++) {

		vector<GestureFrame> g;
		gsd->loadAllGestureFrames(files[i].first, g);
		classDatasets.push_back(TrainingClassDataset(files[i].second, g));

	}

	TrainingStaticRecConf trainingStaticRecConf(path, filename);
	TrainingResult* trainResult = staticRec.train(classDatasets, trainingStaticRecConf);

	cout << "Train result: " << trainResult->trainRate << endl;
	for (unsigned int i = 0; i < trainResult->trainClassResults.size(); i++) {
		cout << "Train result for \""
				<< trainResult->trainClassResults[i].className << "\" class: "
				<< trainResult->trainClassResults[i].classTrainRate << endl;
	}


}


//void Learning::learnDynamic()
