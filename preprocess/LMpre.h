/*
 * LMpre.h
 *
 *  Created on: 04-01-2014
 *      Author: oli
 */

#ifndef LMPRE_H_
#define LMPRE_H_

#include <iostream>
#include <vector>
#include <set>

#include "Model/GestureFinger.h"
#include "Model/GestureFrame.h"
#include "Model/GestureHand.h"
#include "Model/Vertex.h"

#include "StorageDriver/BinaryFileStorageDriver.h"
#include "StorageDriver/GestureStorageDriver.h"


namespace LMpre {

using namespace std;

class LMpre {

public:
	LMpre(char *filepath, int radius);
	vector<GestureFrame> process();
	void displayRes();
	vector<GestureFrame> outFrames;

private:
	vector<GestureFrame> frames;

	BinaryFileStorageDriver input;
	BinaryFileStorageDriver output;
	vector < vector<int> > fids;
	set<int> uniqueFingers;

	int radius;

	bool checkNear(int frameId, int fingerId);
	bool isThereFinger(int fingerId, int frame);
	Vertex weightedAverage(Vertex v1, Vertex v2, int w1, int w2);
	GestureFinger averageFingers(GestureFinger *f1, GestureFinger *f2, int d1, int d2);
	GestureFinger interpolateFinger(int fingerId, int frame);
	GestureFinger cloneFinger(GestureFinger *f);
	GestureFinger* getFinger(int fingerId, int frame);
	int getHandIdFromFinger(int fingerId, int frame, bool deep);
	GestureHand copyHand(GestureHand *source);
	void fillFidVectors();

};


} /* namespace LMpre */
#endif /* LMPRE_H_ */
