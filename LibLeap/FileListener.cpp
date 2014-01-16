/*
 * FileListener.cpp
 *
 *  Created on: 11-01-2014
 *      Author: oli
 */

#include "FileListener.h"

FileListener::FileListener() {



}

void FileListener::attachToProcess(LeapProcess *lp) {
	process = lp;
}


void FileListener::readFile(string filePath) {

	cout << "reading frames..." << endl;

	BinaryFileStorageDriver input;
	input.loadAllGestureFrames(filePath, frames);

	pthread_create(&frameLoaderThread, NULL, &FileListener::runLoader, this);

}

void *FileListener::runLoader(void *c) {
	return ((FileListener *)c)->frameLoader();
}

void *FileListener::frameLoader(void) {

	cout << "Feeding data!" << endl;

//
	process->loadFrame(&frames[0]);
	pthread_mutex_unlock(&process->mutexStat);
	pthread_mutex_unlock(&process->mutexDyn);
//

//	usleep(5000000);

	for (unsigned int i = 1; i < frames.size(); ++i) {
		cout << "Loading frame " << i << "/" << frames.size() << endl;
//		usleep(1000000);

//		if(pthread_mutex_trylock(&process->finished) != 0) continue;

//		cout << "===============WAITING" << endl;
		pthread_mutex_lock(&process->finished);
		pthread_mutex_lock(&process->finished);
//		cout << "Loading!" << endl;

		usleep(2000);

		process->loadFrame(&frames[i]);

		pthread_mutex_unlock(&process->mutexStat);
		pthread_mutex_unlock(&process->mutexDyn);


	}

}
