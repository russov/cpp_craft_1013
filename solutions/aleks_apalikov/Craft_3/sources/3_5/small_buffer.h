#ifndef _SMALL_BUFFER_
#define _SMALL_BUFFER_

#include <vector>
#include <stock_data.h>
#include <iostream>
#include <fstream>
#include "boost/shared_ptr.hpp"
#include <sstream>
#include "Trade.h"
#include "boost/thread.hpp"

typedef unsigned int UINT32;
using namespace std;
typedef vector<size_t> messages_t;
class processor{
	string& num;
	static size_t count;

	boost::mutex& max_mut;
	static size_t max_time;
	messages_t numBytes; //used number of Bytes of idx type
	messages_t messages;
	boost::mutex & total_mut;
	messages_t& total_msgs; //total used Bytes
	bool fileErr;

public:
	static const UINT32 TotalTypes;
	processor(string& name, boost::mutex & mut, messages_t msgs, boost::mutex & max_m):num (name), total_mut(mut), total_msgs(msgs), 
		max_mut(max_m)  													// "YZ"			
	{
		numBytes.resize(TotalTypes + 1, 0);
		messages.resize(TotalTypes + 1, 0);
	}
	~processor()
	{
		if(count == 0)
		{
			stringstream output;
			output << BINARY_DIR << "/output" << ".txt";
			ofstream outp( output.str().c_str(), fstream::binary | fstream::out);

			int type = 0;
			for(vector<size_t>::iterator tb = messages.begin(); tb != messages.end(); ++tb)
			{
				if (*tb == 0)
				{
					type++;
					continue;
				}
				binary_reader::write_binary(outp, type);
				double val = (double)*tb/max_time;
				binary_reader::write_binary(outp, val);
				type++;
			}
		}
	}
	int process_data();
	void operator() ()
	{
		process_data();
	}
};
#endif