#pragma once
#include <fstream>
#include <iostream>

using namespace std;
class config
{
	string file_name;
	ifstream fs;
public:
	config(string& name):file_name(name)
	{
		fs.open(file_name.c_str());
		if(!fs.is_open())
		{
			cout<<"file was not open!";
		}

	}
	~config(void);
};
