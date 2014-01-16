#include "BinaryFileStorageDriver.h"

bool BinaryFileStorageDriver::openConnection(const std::string path, bool withTrunc)
{
	if (!file.is_open())
	{
		if (withTrunc)
		{
			file.open(path.c_str(),
				std::fstream::in | std::fstream::out | std::fstream::trunc);
		}
		else
		{
			file.open(path.c_str(),
				std::fstream::in | std::fstream::out);
		}
		return file.is_open();
	}
	
	return true;
}

void BinaryFileStorageDriver::closeConnection()
{
	if (file.is_open())
	{
		file.close();
	}
}

bool BinaryFileStorageDriver::saveGestureFrame(GestureFrame gestureFrame)
{
	if (file.is_open())
	{
		file<<gestureFrame.getTimestamp();
		for (int i=0; i<gestureFrame.getHandCount(); i++)
		{
			GestureHand *gHand = gestureFrame.getHand(i);
			file<<"#"<<gHand->getId();
			file<<" "<<gHand->getPalmPosition();
			file<<" "<<gHand->getStabilizedPalmPosition();
			file<<" "<<gHand->getPalmNormal();
			file<<" "<<gHand->getDirection();
			
			for (int j=0; j<gHand->getFingerCount(); j++)
			{
				GestureFinger *gFinger = gHand->getFinger(j);
				file<<"f"<<gFinger->getId();
				file<<" "<<gFinger->getTipPosition();
				file<<" "<<gFinger->getStabilizedTipPosition();
				file<<" "<<gFinger->getDirection();
				file<<" "<<gFinger->getLength();
				file<<" "<<gFinger->getWidth();
			}
		}
		file<<std::endl;
		
		return true;
	}
	
	return false;
}

bool BinaryFileStorageDriver::loadGestureFrame(GestureFrame &gestureFrame)
{
	if (file.is_open())
	{	
		std::string gFrameLine;
		std::getline(file, gFrameLine);

		if (file.eof())
			return false;

		gFrameLine += '\n';
		
		bool isEnd = false;
			
		this->currState = TIMESTAMP_STATE;

		for (int i=0; i<gFrameLine.size() && !isEnd; i++)
		{	
			char currSign = gFrameLine[i];
			try
			{		
				switch(currState)
				{
					case TIMESTAMP_STATE:
					    if (tryChangeState(currSign, '#', HAND_ID_STATE)
					    	|| tryChangeState(currSign, '\n', END_PARSE))
					    {
					    	gestureFrame.setTimestamp(strToFloat(parameter));
					    	clearParameters();
					    	break;
					    }
					    parameterStateAction(currSign);
					    break;
					case HAND_ID_STATE:
					    if (tryChangeState(currSign, ' ', HAND_PALM_POSITION_STATE))
					    {
					    	gestureHand.clear();
					    	gestureHand.setId(strToInt(parameter));
					    	clearParameters();
					    	break;
					    }
					    parameterStateAction(currSign);
					    break;
					case HAND_PALM_POSITION_STATE:
					    if (tryChangeState(currSign, ' ', HAND_STABILIZED_PALM_POSITION_STATE))
					    {
					    	gestureHand.setPalmPosition(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_STABILIZED_PALM_POSITION_STATE:
					    if (tryChangeState(currSign, ' ', HAND_PALM_NORMAL))
					    {
					    	gestureHand.setStabilizedPalmPosition(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_PALM_NORMAL:
					    if (tryChangeState(currSign, ' ', HAND_DIRECTION))
					    {
					    	gestureHand.setPalmNormal(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_DIRECTION:
					    if (tryChangeState(currSign, '\n', END_PARSE)
					    	|| tryChangeState(currSign, '#', HAND_ID_STATE))
					    {
					    	gestureHand.setDirection(getVertexParameter());
					    	gestureFrame.addHand(gestureHand);
					    	clearParameters();
					    	break;
					    }
					    if (tryChangeState(currSign, 'f', HAND_FINGER_ID))
					    {
					    	gestureHand.setDirection(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_FINGER_ID:
					    if (tryChangeState(currSign, ' ', HAND_FINGER_TIP_POSITION))
					    {
					    	gestureFinger.clear();
					    	gestureFinger.setId(strToInt(parameter.c_str()));
					    	clearParameters();
					    	break;
					    }
					    parameterStateAction(currSign);
					    break;
					case HAND_FINGER_TIP_POSITION:
					    if (tryChangeState(currSign, ' ', HAND_FINGER_STABILIZED_TIP_POSITION))
					    {
					    	gestureFinger.setTipPosition(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_FINGER_STABILIZED_TIP_POSITION:
					    if (tryChangeState(currSign, ' ', HAND_FINGER_DIRECTION))
					    {
					    	gestureFinger.setStabilizedTipPosition(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_FINGER_DIRECTION:
					    if (tryChangeState(currSign, ' ', HAND_FINGER_LENGTH))
					    {
					    	gestureFinger.setDirection(getVertexParameter());
					    	clearParameters();
					    	break;
					    }
					    vertexParameterStateAction(currSign, ';');
					    break;
					case HAND_FINGER_LENGTH:
					    if (tryChangeState(currSign, ' ', HAND_FINGER_WIDTH))
					    {
					    	gestureFinger.setLength(strToFloat(parameter));
					    	clearParameters();
					    	break;
					    }
					    parameterStateAction(currSign);
					    break;
					case HAND_FINGER_WIDTH:
					    if (tryChangeState(currSign, '\n', END_PARSE)
					    	|| tryChangeState(currSign, '#', HAND_ID_STATE))
					    {
					    	gestureFinger.setWidth(strToFloat(parameter));
					    	gestureHand.addFinger(gestureFinger);
					    	gestureFrame.addHand(gestureHand);
					    	clearParameters();
					    	break;
					    }
					    if (tryChangeState(currSign, 'f', HAND_FINGER_ID))
					    {
					    	gestureFinger.setWidth(strToFloat(parameter));
					    	gestureHand.addFinger(gestureFinger);
					    	clearParameters();
					    	break;
					    }
					    parameterStateAction(currSign);
					    break;
					case END_PARSE:
					    isEnd = true;
					    break;
				}
			}
			catch(std::string & e)
			{
				std::cout<<e<<std::endl;
				return false;
			}
		}
	
		return true;
	}
	
	return false;
}

bool BinaryFileStorageDriver::tryChangeState(char currSign, char endSign, ParseState nextState)
{
	if (currSign == endSign)
	{
		currState = nextState;
		return true;
	}
	
	return false;
}

void BinaryFileStorageDriver::parameterStateAction(char currSign)
{	
	parameter += currSign;
}

void BinaryFileStorageDriver::vertexParameterStateAction(char currSign, char separatorSign)
{
	if (currSign == separatorSign)
	{
		vertexParameterIndex++;
		return;
	}
	
	vertexParameter[vertexParameterIndex] += currSign;
}

void BinaryFileStorageDriver::clearParameters()
{
	this->parameter.clear();
	this->vertexParameter[0].clear();
	this->vertexParameter[1].clear();
	this->vertexParameter[2].clear();
	this->vertexParameterIndex = 0;
}

int BinaryFileStorageDriver::strToInt(std::string value)
{
	std::istringstream ss(value);
	int result;
	if (!(ss >> result))
	{
		throw std::string("Cannot convert to int value: \"") + value + "\"";
	}
	
	return result;
}
	
float BinaryFileStorageDriver::strToFloat(std::string value)
{
	std::istringstream ss(value);
	float result;
	if (!(ss >> result))
	{
		throw std::string("Cannot convert to float value: \"") + value + "\"";
	}
	
	return result;
}
