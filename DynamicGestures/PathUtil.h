#ifndef PATHUTIL_H_INCLUDED
#define PATHUTIL_H_INCLUDED

#include <string>

std::string getFileNameFromPath(const std::string& fullPath)
{
	int i;
	for (i = fullPath.size() - 1; i >= 0 && fullPath[i] != '\\' && fullPath[i] != '/'; i--);

	return fullPath.substr(i + 1, fullPath.size() - i - 1);
}

std::string getFileNameFromPath(const char *fullPath)
{
	std::string stringFullPath = fullPath;
	return getFileNameFromPath(stringFullPath);
}

#endif  // PATHUTIL_H_INCLUDED
