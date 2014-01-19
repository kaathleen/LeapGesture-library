#include "LogUtil.h"

bool LogUtil::debugQuietMode = true;
bool LogUtil::errorQuietMode = true;
bool LogUtil::infoQuietMode = true;
std::ostream* LogUtil::outStream = &std::cout;

LogUtil::LogUtil(std::string className) {
	this->className = className;
}

LogUtil::~LogUtil() {}

void LogUtil::setOutStream(std::ostream* outStream) {
	this->outStream = outStream;
}

void LogUtil::debug(std::string msg) {
	if (!debugQuietMode) {
		*outStream<<"DEBUG";
		printMsg(msg);
	}
}

void LogUtil::error(std::string msg) {
	if (!errorQuietMode) {
		*outStream<<"ERROR";
		printMsg(msg);
	}
}

void LogUtil::info(std::string msg) {
	if (!infoQuietMode) {
		*outStream<<"INFO";
		printMsg(msg);
	}
}

std::ostream& LogUtil::sdebug() {
	if (!debugQuietMode) {
		*outStream<<"DEBUG";
		*outStream<<" "<<className<<" - ";
	}

	return *outStream;
}

std::ostream& LogUtil::serror() {
	if (!errorQuietMode) {
		*outStream<<"ERROR";
		*outStream<<" "<<className<<" - ";
	}

	return *outStream;
}

std::ostream& LogUtil::sinfo() {
	if (!infoQuietMode) {
		*outStream<<"INFO";
		*outStream<<" "<<className<<" - ";
	}

	return *outStream;
}

void LogUtil::printMsg(std::string msg) {
	*outStream<<" "<<className<<" - ";
	*outStream<<msg<<"\n";
}

