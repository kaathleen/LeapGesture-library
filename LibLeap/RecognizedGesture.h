#ifndef RECOGNIZEDGESTURE_H_
#define RECOGNIZEDGESTURE_H_

class RecognizedGestureListener {
public:
//	RecognizedGesture();
//	virtual ~RecognizedGesture()=0;
	virtual void onDynamicRecognized() = 0;
	virtual void onStaticRecognized() = 0;
};

#endif /* RECOGNIZEDGESTURE_H_ */

