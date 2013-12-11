#include "GestureFrame.h"

GestureFrame::GestureFrame(double timestamp)
{
	this->timestamp = timestamp;
}

void GestureFrame::addHand(GestureHand gestureHand)
{
	this->hands.push_back(gestureHand);
}

GestureHand* GestureFrame::getHand(int handIndex)
{
	if (handIndex < hands.size()) {
		return &(hands[handIndex]);
	}
	
	return NULL;
}

void GestureFrame::clear()
{
	timestamp = 0;
	hands.clear();
}
