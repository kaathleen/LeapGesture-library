#include <string>

#ifndef PATHUTIL_H_
#define PATHUTIL_H_

class PathUtil {
public:
	PathUtil();

	static std::string getFileNameFromPath(const std::string& path);

	static std::string getFileNameFromPath(const char *path);

	static std::string combinePathAndFileName(const std::string& path,
			const std::string& fileName);

	static std::string combinePathFileNameAndExt(const std::string& path,
			const std::string& fileName, const std::string& extension);

private:
#ifdef WIN32
	const static char DIRECTORY_SEPARATOR = '\\';
#else
	const static char DIRECTORY_SEPARATOR = '/';
#endif
};

#endif /* PATHUTIL_H_ */
