//check.cpp
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	fstream f(argv[1], ios::in);
	if(!f.good())
	{
		cout<<argv[1]<<"open failed!"<<endl;
	}
	
	int e=-1,etmp=-1;
	char s;
	while(true)
	{
		f>>e>>s>>e;
		if(f.fail() )
			break;
		//cout<<e<<endl;
		if(e <= etmp )
		{
			cout<<"check error: last="<<etmp<<",current="<<e<<endl;
			//break;
		}
		etmp = e;
	}

	return 0;
}