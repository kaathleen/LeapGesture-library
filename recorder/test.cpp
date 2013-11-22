#include "LMReader.h"

int main(int argc, char **argv) {

	if(argc<2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}


	// create Reader object and read given file
	LMReader *r = new LMReader(argv[1]);

	// if there's a frame
	while(r->nextFrame()) {

		// get no. of fingers
		printf("%d fingers: ", r->fingerCount);

		// show position of fingertip
		// getfinger(FINGERID, PARAM)
		// FINGERID - id of finger in view - supports up to 20 fingers in field of view (is that enough?)
		// PARAM - available are TIPX, TIPY, TIPZ for fingertip and DIRX, DIRY, DIRZ

		for(int i=0; i<r->fingerCount; i++) {
			printf("X: %.4f / Y: %.4f / Z: %.4f # ", r->getFinger(i, TIPX), r->getFinger(i, TIPY), r->getFinger(i, TIPZ));
		}

		printf("\n");

	}

	return 0;
}



