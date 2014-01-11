#ifndef GESTURESTORAGEDRIVER_H_INCLUDED
#define GESTURESTORAGEDRIVER_H_INCLUDED

#include <string>
#include <vector>

#include "../Model/GestureFrame.h"

class GestureStorageDriver {
public:
	virtual bool openConnection(const std::string path, bool withTrunc) = 0;
	virtual void closeConnection() = 0;
	
	virtual bool saveGestureFrame(const GestureFrame gestureFrame) = 0;
	
	virtual bool loadGestureFrame(GestureFrame &gestureFrame) = 0;
	virtual bool loadAllGestureFrames(const std::string path, std::vector<GestureFrame> &gestureFrames)
	{
		if (openConnection(path, false))
		{
			GestureFrame gestureFrame;
			while(loadGestureFrame(gestureFrame))
			{
				gestureFrames.push_back(gestureFrame);
				gestureFrame.clear();
			}
			
			closeConnection();
			
			return true;
		}
		
		return false;
	}
};

#endif // GESTURESTORAGEDRIVER_H_INCLUDED
