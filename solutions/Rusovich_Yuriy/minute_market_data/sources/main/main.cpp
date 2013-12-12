
#include <string.h>
#include <signal.h>

#include <boost/thread.hpp>

#include <fstream>
#include <sstream>

#include "common_data.h"
#include "minute_calculator.h"

typedef std::vector < common_data::minute_datafeed > vector_minute_datafeed;


void save_datafeed( const vector_minute_datafeed& minute_datafeeds )
{
	std::ofstream out;
	
	vector_minute_datafeed::iterator iterator;

	/*for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
	{

	}*/

    out.close();
}

void process()
{
	minute_calculator::minute_calculator_process calculator;
	//multicast_communication communication;
	
	while(1)
	{
		common_data::quote_data quote;

		try
		{
			calculator.add_quote_data(quote);
		} 
		catch (...)
		{
		
		}


		boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
	}
}		

static void button_handler( int )
{
	std::cout << "!!" << std::endl;
	exit( 0 );
}

int main()
{
	signal( SIGINT, button_handler );

	boost::thread head_thread( boost::bind( &process ));
	head_thread.join();
}
