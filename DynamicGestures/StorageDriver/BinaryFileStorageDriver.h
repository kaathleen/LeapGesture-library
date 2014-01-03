#ifndef BINARYFILESTORAGEDRIVER_H_INCLUDED
#define BINARYFILESTORAGEDRIVER_H_INCLUDED

#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include "../Model/GestureFrame.h"
#include "../Model/GestureHand.h"
#include "../Model/GestureFinger.h"
#include "../Model/Vertex.h"

#include "GestureStorageDriver.h"

/**
BinaryFileStorageDriver

File format:
Line represents one frame.

Structure of frame (values in square brackets are frame parameters):
[timestamp]#[hand1_id] [hand1_palmPositionX];[hand1_palmPositionY];[hand1_palmPositionZ] [hand1_stabilizedPalmPositionX];[hand1_stabilizedPalmPositionY];[hand1_stabilizedPalmPositionZ] [hand1_palmNormalX];[hand1_palmNormalY];[hand1_palmNormalZ] [hand1_directionX];[hand1_directionY];[hand1_directionZ]f[hand1_finger1_id] [hand1_finger1_tipPositionX];[hand1_finger1_tipPositionY];[hand1_finger1_tipPositionZ] [hand1_finger1_stabilizedTipPositionX];[hand1_finger1_stabilizedTipPositionY];[hand1_finger1_stabilizedTipPositionZ] [hand1_finger1_directionX];[hand1_finger1_directionY];[hand1_finger1_directionZ] [hand1_finger1_length] [hand1_finger1_width]f...#...\n
Timestamp and hands are separated by '#'. In specified hand occurs hand parameters and fingers. Hand parameters are separated by space, and finger are separated by 'f' (hands parameters and fingers are separated by 'f' too). Specified finger has parameters, which are splited by ' '. Values ​​in the trivalent parameters are separated by a semicolon.
**/

class BinaryFileStorageDriver : public GestureStorageDriver {
public:
	bool openConnection(std::string path, bool withTrunc);
	
	void closeConnection();
	
	bool saveGestureFrame(GestureFrame gestureFrame);
	
	bool loadGestureFrame(GestureFrame &gestureFrame);
private:
	enum ParseState
	{
		TIMESTAMP_STATE,
		HAND_ID_STATE,
		HAND_PALM_POSITION_STATE,
		HAND_STABILIZED_PALM_POSITION_STATE,
		HAND_PALM_NORMAL,
		HAND_DIRECTION,
		HAND_FINGER_ID,
		HAND_FINGER_TIP_POSITION,
		HAND_FINGER_STABILIZED_TIP_POSITION,
		HAND_FINGER_DIRECTION,
		HAND_FINGER_LENGTH,
		HAND_FINGER_WIDTH,
		END_PARSE
	};
	
	std::fstream file;
	
	ParseState currState;
	
	std::string parameter;
	std::string vertexParameter[3];
	int vertexParameterIndex;
	GestureHand gestureHand;
	GestureFinger gestureFinger;
	
	bool tryChangeState(char currSign, char endSign, ParseState nextState);
	
	void parameterStateAction(char currSign);
	
	void vertexParameterStateAction(char currSign, char separatorSign);
	
	void clearParameters();
	
	Vertex getVertexParameter()
	{
		return Vertex(
	    		strToFloat(vertexParameter[0]),
	    		strToFloat(vertexParameter[1]),
	    		strToFloat(vertexParameter[2])
	    	);
	}
	
	int strToInt(std::string value);
	
	float strToFloat(std::string value);
};

#endif // BINARYFILESTORAGEDRIVER_H_INCLUDED
