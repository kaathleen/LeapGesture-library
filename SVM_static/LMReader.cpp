#include "LMReader.h"

LMReader::LMReader(string lmrfile) {

	infile.open(lmrfile.c_str());

}

bool LMReader::nextFrame() {

	bool status = getline(infile, buf);
	fillFields();

	return status;

}

double LMReader::getFinger(int finger, int i) {

	return fingers[finger*6+i];

}

void LMReader::fillFields() {

	p_begin = 0;
	p_end = 0;

	while(buf[p_end] != '#') p_end++;

	//get timestamp
	strncpy(numbuf, buf.c_str(), p_end);
	time_ms = atof(numbuf);

	//
	p_end++;
	p_begin = p_end;

	int fieldCount = -1;

	while(buf[p_end] != 0x00) {

		fieldCount++;

		// move cursor to end of number
		while(buf[p_end] != 0x00 && (buf[p_end] != ';' && buf[p_end] != '#')) p_end++;

		// substring
		strncpy(numbuf, &buf[p_begin], p_end-p_begin);

		// read double from buffer
		fingers[fieldCount] = atof(numbuf);

		p_end++;
		p_begin = p_end;

	}

	fingerCount = (fieldCount+1)/6;



}
