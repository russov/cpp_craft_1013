#include "test_registrator.h"
#include <market_data_receiver.h>
#include <data_processor.h>
#include <sstream>
#include <fstream>

void multicast_communication::tests_::market_data_receiver_tests()
{
	{

		const size_t trade_thread_size = 8;
		const size_t quote_thread_size = 8;
		addresses_and_ports trades_adr, quotes_adr;
		trades_adr.push_back(address_and_port("233.200.79.128",62128));
		/*trades_adr.push_back(address_and_port("233.200.79.129",62129));
		trades_adr.push_back(address_and_port("233.200.79.130",62130));
		trades_adr.push_back(address_and_port("233.200.79.131",62131));
		trades_adr.push_back(address_and_port("233.200.79.132",62132));
		trades_adr.push_back(address_and_port("233.200.79.133",62133));
		trades_adr.push_back(address_and_port("233.200.79.134",62134));
		trades_adr.push_back(address_and_port("233.200.79.135",62135));

		*/
		quotes_adr.push_back(address_and_port("233.200.79.0",61000));
		/*
		quotes_adr.push_back(address_and_port("233.200.79.1",61001));
		quotes_adr.push_back(address_and_port("233.200.79.2",61002));
		quotes_adr.push_back(address_and_port("233.200.79.3",61003));
		quotes_adr.push_back(address_and_port("233.200.79.4",61004));
		quotes_adr.push_back(address_and_port("233.200.79.5",61005));
		quotes_adr.push_back(address_and_port("233.200.79.6",61006));
		quotes_adr.push_back(address_and_port("233.200.79.7",61007));*/
		
		config_reader* pcr = new config_reader(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

		std::stringstream ss;
		multicast_communication::data_processor processor(ss);
		multicast_communication::market_data_receiver receiver(processor, pcr);
		
		receiver.run();
		boost::this_thread::sleep_for( boost::chrono::milliseconds( 4000 ) ); 
		receiver.stop();
		
	}
	
}