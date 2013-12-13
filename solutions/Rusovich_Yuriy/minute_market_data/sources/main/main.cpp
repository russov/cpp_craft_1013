
#include <string>
#include <signal.h>
#include <vector>

#include <boost/thread.hpp>

#include <fstream>
#include <sstream>

#include "common_data.h"
#include "minute_calculator.h"

typedef std::vector < common_data::minute_datafeed > vector_minute_datafeed;

std::map<std::string, std::ofstream *> output_files;
minute_calculator::minute_calculator_process calculator;
//multicast_communication communication;

void save_datafeed(common_data::minute_datafeed& datafeed )
{
	if (output_files.find(datafeed.stock_name) == output_files.end())
	{
		std::string fileName =  SOURCE_DIR + std::string( "/" ) + std::string( datafeed.stock_name ) + ".dat";
		std::ofstream *output_file = new std::ofstream(fileName.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::trunc  );
		output_files.insert(std::pair<std::string, std::ofstream *>(std::string(datafeed.stock_name), output_file));
	}
	
	if (output_files[std::string(datafeed.stock_name)]->is_open())
		output_files[std::string(datafeed.stock_name)]->write(reinterpret_cast<char*>(&datafeed), sizeof(datafeed));
}

void process()
{
	while(1)
	{
		common_data::quote_data quote;
		quote.bid_price = 4;
		quote.offer_price = 6;
		strcpy(quote.security_symbol, "55");
		quote.time = 656;

		common_data::trade_data trade;

		trade.price = 34;
		strcpy(trade.security_symbol, "55");
		trade.time = 656;

		common_data::quote_data quote1;
		quote1.bid_price = 324;
		quote1.offer_price = 23523513;
		strcpy(quote1.security_symbol, "55");
		quote1.time = 10000;


		try
		{
			calculator.add_quote_data(quote);
			calculator.add_trade_data(trade);
			calculator.add_quote_data(quote1);
			
		} 
		catch ( std::logic_error& )
		{
		}

		common_data::minute_datafeed data = calculator.get_minute_datafeed_one();
		if ( data.minute != 0 )
			save_datafeed(data);

		boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
	}
}		

static void button_handler( int )
{
	vector_minute_datafeed minute_datafeeds = calculator.get_minute_datafeed();
	calculator.stop();
	
	while (!minute_datafeeds.empty())
	{
		save_datafeed(minute_datafeeds.back());
		minute_datafeeds.pop_back();
	}
	
	for(std::map < std::string, std::ofstream *>::iterator it = output_files.begin();
			it != output_files.end(); )
	{
		it->second->close();
		delete it->second;
		it = output_files.erase(it);
	}

	exit( 0 );
}

int main()
{
	signal( SIGINT, button_handler );

	boost::thread head_thread( boost::bind( &process ));
	head_thread.join();
}
