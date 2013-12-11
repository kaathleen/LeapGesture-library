#ifndef GESTUREHAND_H_INCLUDED
#define GESTUREHAND_H_INCLUDED

#include <algorithm>
#include <vector>

#include "../Model/GestureFinger.h"
#include "../Model/Vertex.h"

class GestureHand
{
public:
	GestureHand(){}
	
    	GestureHand(int id, Vertex palmPosition, Vertex stabilizedPalmPosition,
		Vertex palmNormal, Vertex direction);
	
	int getId() const
	{
		return this->id;
	}
	
	void setId(int id)
	{
		this->id = id;
	}
    	
    	Vertex getPalmPosition() const
    	{
    		return this->palmPosition;
    	}
    	
    	void setPalmPosition(Vertex palmPosition)
    	{
    		this->palmPosition = palmPosition;
    	}
    	
    	Vertex getStabilizedPalmPosition() const
    	{
    		return this->stabilizedPalmPosition;
    	}
    	
    	void setStabilizedPalmPosition(Vertex stabilizedPalmPosition)
    	{
    		this->stabilizedPalmPosition = stabilizedPalmPosition;
    	}
    	
    	Vertex getPalmNormal() const
    	{
    		return this->palmNormal;
    	}
    	
    	void setPalmNormal(Vertex palmNormal)
    	{
    		this->palmNormal = palmNormal;
    	}
    	
	Vertex getDirection() const
	{
		return this->direction;
	}
	
	void setDirection(Vertex direction)
	{
		this->direction = direction;
	}

	void addFinger(GestureFinger finger);
	
	GestureFinger* getFinger(int fingerIndex);

	int getFingerCount() const
	{
		return this->fingers.size();
	}
	
	float getOrderValue()
	{
		return this->orderValue;
	}
	
	void setOrderValue(float orderValue)
	{
		this->orderValue = orderValue;
	}
	
	void sortFingers()
	{
		std::sort(fingers.begin(), fingers.end());
	}
	
	bool operator< (const GestureHand &other) const
	{
		return this->orderValue < other.orderValue;
    	}
	
	void clear();

private:
	int id;
	Vertex palmPosition;
	Vertex stabilizedPalmPosition;
	Vertex palmNormal;
	Vertex direction;
	
	std::vector<GestureFinger> fingers;
	
	float orderValue;
};

#endif  // GESTUREHAND_H_INCLUDED
