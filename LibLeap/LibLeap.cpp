#include "LibLeap.h"

using namespace std;

LibLeap::LibLeap() {


}

int LibLeap::b() {
	c = 4;
	return 0;
}

void *LibLeap::test_thread(void * obj) {

	cout << "IM ALIVE!!!!";
	c = 6+2;

}

void LibLeap::start() {

	pthread_t t1;



//	pthread_create(&t1, NULL, (void* (*)(void*))&LibLeap::test_thread, NULL);

}
