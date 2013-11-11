#ifndef GESTUREFRAME_H_INCLUDED
#define GESTUREFRAME_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Util/Position.h"

//========================================================================== ====
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class GestureFrame
{
public:
    //==============================================================================
    GestureFrame();

	void addFingerTip(Position fingerTipPosition);
    
	Position* getFingerTipPositions()
	{
		return this->fingerTipPositions;
	}

	Position getFingerTipPosition(int fingerIndex)
	{
		return this->fingerTipPositions[fingerIndex];
	}

	int getFingerCount()
	{
		return this->fingerCount;
	}

private:
    //==============================================================================
	const static int FINGER_MAX_COUNT = 20;

	Position fingerTipPositions[FINGER_MAX_COUNT];
	int fingerCount;
	
};

#endif  // GESTUREFRAME_H_INCLUDED