#include "test_registrator.h"
#include <market_data_receiver.h>
#include <data_processor.h>
#include <sstream>

void multicast_communication::tests_::market_data_receiver_tests()
{
	{
		std::stringstream ss;
		multicast_communication::data_processor processor(ss);
		multicast_communication::market_data_receiver receiver(processor);


		//receiver.run();




		//receiver.stop();
	}
	
}