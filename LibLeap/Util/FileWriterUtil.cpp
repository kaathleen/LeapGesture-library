#include "FileWriterUtil.h"

FileWriterUtil::FileWriterUtil(const std::string filePath, bool quietMode) {
	this->filePath = filePath;
	this->quietMode = quietMode;
}

void FileWriterUtil::open()
{
	if (!file.is_open() && quietMode == false)
	{
		file.open(filePath.c_str(), std::fstream::out | std::fstream::trunc);
	}
}

void FileWriterUtil::FileWriterUtil::close()
{
	if (file.is_open() && quietMode == false)
	{
		file.close();
	}
}

