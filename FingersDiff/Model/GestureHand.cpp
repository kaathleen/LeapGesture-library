#include "GestureHand.h"

GestureHand::GestureHand(int id, Vertex palmPosition, Vertex stabilizedPalmPosition,
		Vertex palmNormal, Vertex direction)
{
	this->id = id;
	this->palmPosition = palmPosition;
	this->stabilizedPalmPosition = stabilizedPalmPosition;
	this->palmNormal = palmNormal;
	this->direction = direction;
}

void GestureHand::addFinger(GestureFinger finger)
{
	fingers.push_back(finger);
}

GestureFinger* GestureHand::getFinger(int fingerIndex)
{
	if (fingerIndex < fingers.size()) {
		return &(fingers[fingerIndex]);
	}
	
	return NULL;
}

void GestureHand::clear()
{
	id = 0;
	palmPosition.clear();
	stabilizedPalmPosition.clear();
	palmNormal.clear();
	direction.clear();

	fingers.clear();
}
