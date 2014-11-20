#ifndef SPLITOR_H
#define SPLITOR_H
 
#include <vector>

#ifndef UTILITY_H
#include "utility.h"
#endif

using namespace std;

class Splitor
{
public:
	typedef vector<string> value_type;

public:
	static value_type Split(char* splitor, char *src)
	{
		value_type results;
		char *p;
        p = strtok(src,splitor);
        while(p)
        {
                results.push_back(p);
                p=strtok(NULL,splitor);
        }

		return results;

	}

	static vector<int> Split2Int(char *splitor, char *src)
	{
		vector<int> results;
		char *p;
        p = strtok(src,splitor);
        while(p)
        {
				int n;
				string2t(string(p), n);
                results.push_back(n);
                p=strtok(NULL,splitor);
        }

		return results;
	}

};

#endif /*SPLITOR_H*/