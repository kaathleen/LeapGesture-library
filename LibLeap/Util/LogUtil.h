#include <string>
#include <iostream>

#ifndef LOGUTIL_H_
#define LOGUTIL_H_

class LogUtil {
public:
	LogUtil(std::string className);
	~LogUtil();

	void setOutStream(std::ostream* outStream);

	void debug(std::string msg);

	void error(std::string msg);

	void info(std::string msg);

	std::ostream& sdebug();

	std::ostream& serror();

	std::ostream& sinfo();

	static void setQuietMode(bool quietMode) {
		debugQuietMode = quietMode;
		errorQuietMode = quietMode;
		infoQuietMode = quietMode;
	}
	static void setDebugQuietMode(bool quietMode) {
		debugQuietMode = quietMode;
	}
	static void setErrorQuietMode(bool quietMode) {
		errorQuietMode = quietMode;
	}
	static void setInfoQuietMode(bool quietMode) {
		infoQuietMode = quietMode;
	}

private:
	static bool debugQuietMode;
	static bool errorQuietMode;
	static bool infoQuietMode;

	std::string className;
	static std::ostream* outStream;

	void printMsg(std::string msg);
};

#endif /* LOGUTIL_H_ */
