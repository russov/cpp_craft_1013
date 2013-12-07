#ifndef _market_data_processor_
#define _market_data_processor_
#include "boost/thread.hpp"
#include "message_parser.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "config.h"

using namespace std;
class market_data_processor
{
	ofstream outp;
	boost::mutex mtx;
public:
	market_data_processor()
	{
		string str = data_path + "results.txt";
		outp.open( str.c_str() );
		cout << "processor opened file"<<endl;
	}
/*	market_data_processor(ofstream output)
	{
		outp = output;

	}*/
	~market_data_processor()
	{
		close();
	}
	void close()
	{
		outp.close();
	}
	void flush()
	{
		outp.flush();
	}
	int wr_trade(shared_trade trad);
	int wr_trades(vector_messages& msgs);
	int wr_quote(shared_quote quot);
	int wr_quotes(vector_messages& msgs);

};
#endif //_market_data_processor_
