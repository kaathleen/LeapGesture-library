#ifndef GESTUREFINGER_H_INCLUDED
#define GESTUREFINGER_H_INCLUDED

#include "../Model/Vertex.h"

class GestureFinger
{
public:
	GestureFinger() {}

    	GestureFinger(int id, Vertex tipPosition, Vertex stabilizedTipPosition,
    		Vertex direction, float length, float width);
	
	int getId() const
	{
		return this->id;
	}
	
	void setId(int id)
	{
		this->id = id;
	}
    	
    	Vertex getTipPosition() const
    	{
    		return this->tipPosition;
    	}
    	
    	void setTipPosition(Vertex tipPosition)
    	{
    		this->tipPosition = tipPosition;
    	}
    	
    	Vertex getStabilizedTipPosition() const
    	{
    		return this->stabilizedTipPosition;
    	}
    	
    	void setStabilizedTipPosition(Vertex stabilizedTipPosition)
    	{
    		this->stabilizedTipPosition = stabilizedTipPosition;
    	}
    	
    	Vertex getDirection() const
    	{
    		return this->direction;
    	}
    	
    	void setDirection(Vertex direction)
    	{
    		this->direction = direction;
    	}
    	
	float getLength() const
	{
		return this->length;
	}
	
	void setLength(float length)
	{
		this->length = length;
	}
	
	float getWidth() const
	{
		return this->width;
	}
	
	void setWidth(float width)
	{
		this->width = width;
	}
	
	float getOrderValue()
	{
		return this->orderValue;
	}
	
	void setOrderValue(float orderValue)
	{
		this->orderValue = orderValue;
	}
	
	bool operator< (const GestureFinger &other) const
	{
		return this->orderValue < other.orderValue;
    	}
	
	void clear();

private:
	int id;
	Vertex tipPosition;
	Vertex stabilizedTipPosition;
	Vertex direction;
	float length;
	float width;
	
	float orderValue;
};

#endif  // GESTUREFINGER_H_INCLUDED
