/*
 * LMReader.h
 *
 *  Created on: 10-11-2013
 *      Author: oli
 */

#ifndef LMREADER_H_
#define LMREADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#define TIPX 0
#define TIPY 1
#define TIPZ 2
#define DIRX 3
#define DIRY 4
#define DIRZ 5


using namespace std;

class LMReader {

public:

	int fingerCount;
	double time_ms;

	LMReader(string lmrfile);
	double getFinger(int finger, int i);
	bool nextFrame();


private:
	ifstream infile;
	string buf;
	int p_begin, p_end;
	void fillFields();
	char numbuf[50];
	double fingers[121];

};

#endif /* LMREADER_H_ */
