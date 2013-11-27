#ifndef _BINARY_DATAFEED_
#define _BINARY_DATAFEED_
#include <fstream>
#include <map>
#include <vector>
#include "stock_data.h"
#include "boost\thread.hpp"

using namespace std;
class Datafeed
{
	ifstream inp;
	ofstream ofp;
	bool fileErr;
	typedef map<string, vector<binary_reader::stock_data>> map_stock;
	map_stock tasks;

public:
	Datafeed()
	{
		stringstream ss;
		ss << BINARY_DIR << "/" << "input.txt";
		string in_name = ss.str();
		inp.open(in_name.c_str(), fstream::in | fstream::binary);
		
		if(!inp.is_open() )
		{
			cout << "File not found! "<<endl;
			fileErr = true;
			return;
		}
	}
	int run()
	{
		if(fileErr)
			return -1;
		while(true)
		{
			binary_reader::stock_data sd(inp);
			map_stock::iterator it = tasks.find(sd.stock_name_);
			if(it != tasks.end())
			{
				
			}
		}
	}
	int operator() ()
	{

	}
};
#endif