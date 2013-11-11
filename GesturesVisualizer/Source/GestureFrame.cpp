#include "GestureFrame.h"

GestureFrame::GestureFrame()
{
	this->fingerCount = 0;
}

void GestureFrame::addFingerTip(Position fingerTipPosition)
{
	if (fingerCount < FINGER_MAX_COUNT)
	{
		fingerTipPositions[fingerCount] = fingerTipPosition;
		fingerCount++;
	}
}