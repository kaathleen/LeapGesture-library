#ifndef GESTUREFRAME_H_INCLUDED
#define GESTUREFRAME_H_INCLUDED

#include "Header.h"

#include "Util/Vertex.h"

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

	void addFinger(Vertex fingerTipPosition, Vertex fingerVector);
    
	Vertex* getFingersTipPositions()
	{
		return this->fingersTipPositions;
	}

	Vertex* getFingerVectors()
	{
		return this->fingersVectors;
	}

	Vertex getFingerTipPosition(int fingerIndex)
	{
		return this->fingersTipPositions[fingerIndex];
	}

	Vertex getFingerVector(int fingerIndex)
	{
		return this->fingersVectors[fingerIndex];
	}

	int getFingerCount()
	{
		return this->fingerCount;
	}

private:
    //==============================================================================
	const static int FINGER_MAX_COUNT = 20;

	Vertex fingersTipPositions[FINGER_MAX_COUNT];
	Vertex fingersVectors[FINGER_MAX_COUNT];
	int fingerCount;
	
};

#endif  // GESTUREFRAME_H_INCLUDED
