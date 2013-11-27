#ifndef STRINGHELPER_H_INCLUDED
#define STRINGHELPER_H_INCLUDED

class StringHelper
{
public:
    //==============================================================================
    static vector<string>& split(const string &s, char delim, vector<string> &elems) 
	{
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	static vector<string> split(const string &s, char delim) 
	{
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

private:
    //==============================================================================
	
};

#endif //STRINGHELPER_H_INCLUDED
