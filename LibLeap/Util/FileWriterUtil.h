#include <string>
#include <fstream>

#ifndef FILEWRITERUTIL_H_
#define FILEWRITERUTIL_H_

class FileWriterUtil {
public:
	FileWriterUtil(const std::string filePath, bool quietMode = false);

	void open();
	void close();

	template<class T>
	FileWriterUtil & operator<<(const T val);

private:
	std::fstream file;
	std::string filePath;
	bool quietMode;

};

template<class T>
FileWriterUtil & FileWriterUtil::operator<<(const T val)
{
	file<<val;
	return *this;
}

#endif /* FILEWRITERUTIL_H_ */
