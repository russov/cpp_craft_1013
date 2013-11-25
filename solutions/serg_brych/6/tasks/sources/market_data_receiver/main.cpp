#include <iostream>
#include <csignal>
#include <cstdint>
#include <boost/thread.hpp>
#include "market_data_receiver.h"
#include "data_processor.h"


static void signalHandler( int signum )
{
    std::cout << "Prepare to end please wait." << std::endl;
	exit(signum);  
}

int main()
{
	signal(SIGINT, signalHandler);  
	
	multicast_communication::data_processor processor(std::cout);
	multicast_communication::market_data_receiver receive(processor);

	receive.run();
		
	boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));

	receive.stop();
	return 0;
}
