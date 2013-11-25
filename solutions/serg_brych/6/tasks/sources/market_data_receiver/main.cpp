#include <iostream>
#include <csignal>
#include <cstdint>
#include <boost/thread.hpp>
#include "market_data_receiver.h"


static void signalHandler( int signum )
{
    std::cout << "Prepare to end please wait." << std::endl;
	exit(signum);  
}

int main()
{
	signal(SIGINT, signalHandler);  
	while(true)
	{
		std::cout << "Program working....." << std::endl;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}



	return 0;
}
