#ifndef _market_data_processor_
#define _market_data_processor_
#include "boost/thread.hpp"
#include "message_parser.h"
#include <iostream>
#include <iomanip>

using namespace std;
class market_data_processor
{
	ostream& outp;
	boost::mutex mtx;
public:
	market_data_processor(ostream& output): outp ( output)
	{

	}
	int wr_trade(shared_trade trad);
	int wr_quote(shared_quote quot);

};
#endif //_market_data_processor_
