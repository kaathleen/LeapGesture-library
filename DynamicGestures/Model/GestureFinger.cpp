#include "GestureFinger.h"

GestureFinger::GestureFinger(int id, Vertex tipPosition, Vertex stabilizedTipPosition,
	Vertex direction, float length, float width)
{
	this->id = id;
	this->tipPosition = tipPosition;
	this->stabilizedTipPosition = stabilizedTipPosition;
	this->direction = direction;
	this->length = length;
	this->width = width;	
}

void GestureFinger::clear()
{
	this->id = 0;
	this->tipPosition.clear();
	this->stabilizedTipPosition.clear();
	this->direction.clear();
	this->length = 0;
	this->width = 0;	
}
