#include "PathUtil.h"

PathUtil::PathUtil() {

}

std::string PathUtil::getFileNameFromPath(const std::string& path) {
	if (path.length() < 1)
		return "";

	int i;
	for (i = path.size() - 1; i >= 0 && path[i] != DIRECTORY_SEPARATOR; i--)
		;

	return path.substr(i + 1, path.size() - i - 1);
}

std::string PathUtil::getFileNameFromPath(const char *path) {
	std::string stringPath = path;
	return getFileNameFromPath(stringPath);
}

std::string PathUtil::combinePathAndFileName(const std::string& path,
		const std::string& fileName) {
	std::string corrFileName = PathUtil::getFileNameFromPath(fileName);
	int pathLength = path.length();

	std::string corrPath = path;
	if (pathLength > 0 && path[pathLength - 1] != DIRECTORY_SEPARATOR) {
		corrPath += DIRECTORY_SEPARATOR;
	}

	return corrPath + corrFileName;
}

std::string PathUtil::combinePathFileNameAndExt(const std::string& path,
		const std::string& fileName, const std::string& extension)
{
	return combinePathAndFileName(path, fileName + "." + extension);
}
