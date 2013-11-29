#include "GestureFrame.h"

GestureFrame::GestureFrame()
{
	this->fingerCount = 0;
}

void GestureFrame::addFinger(Vertex fingerTipPosition, Vertex fingerVector)
{
	if (fingerCount < FINGER_MAX_COUNT)
	{
		fingersTipPositions[fingerCount] = fingerTipPosition;
		fingersVectors[fingerCount] = fingerVector;
		fingerCount++;
	}
}