#ifndef GESTUREFRAME_H_INCLUDED
#define GESTUREFRAME_H_INCLUDED

#include <algorithm>
#include <vector>

#include "../Model/GestureHand.h"

class GestureFrame
{
public:
	GestureFrame() {}

	GestureFrame(double timestamp);
	
	double getTimestamp() const
	{
		return this->timestamp;
	}
	
	void setTimestamp(double timestamp)
	{
		this->timestamp = timestamp;
	}
	
	void addHand(GestureHand gestureHand);
    
	GestureHand* getHand(int handIndex);
	
	int getHandCount() const
	{
		return hands.size();
	}
	
	void sortHands()
	{
		std::sort(hands.begin(), hands.end());
	}
	
	void clear();

private:
	double timestamp;

	std::vector<GestureHand> hands;
	
};

#endif  // GESTUREFRAME_H_INCLUDED
