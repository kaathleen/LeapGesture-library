/*
 * Learning.h
 *
 *  Created on: 29-01-2014
 *      Author: oli
 */

#ifndef LEARNING_H_
#define LEARNING_H_

#include "includes.h"

using namespace std;

class Learning {
public:
	Learning();
	static void learnStatic(vector<pair<const char*, const char*> > files, char* path, char* filename);
	static void learnDynamic();
};

#endif /* LEARNING_H_ */
