#include "test_registrator.h"
#include <string>
#include "Stock_receiver.h"
#include "config.h"
#include <sstream>
#include "market_data_processor.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>

void service_thread( boost::asio::io_service& service )
{
	service.run();
}

void async_udp::receiver_test()
{
	{
		stock_receiver sr;
		boost::asio::io_service service;
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( "233.200.79.128" ), 62128 ); 
		boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
		//	boost::thread receive_messages( boost::bind( service_thread, boost::ref( service ) ) );

		stringstream ss;
		ss << "EBEO A  003759557N:J_735AVB             0    AAAR B30000012127000000001D0000012137000000001     A   62TB00012130001 BB00012137001 "
			<<"EDEO A  003759121P:J_428AINR  D00352000001 F00354300001 02"
			<<"LDEO A  003759122N:J_432ALJR  F00124900003 D00125100001 02";
		string str = ss.str();
		socket.send_to( boost::asio::buffer( str.c_str(), str.size() ), endpoint );
		while( sr.wait_some_data() == -1 )
		{
			socket.send_to( boost::asio::buffer( str.c_str(), str.size() ), endpoint );
			boost::this_thread::sleep_for( boost::chrono::nanoseconds( 100 ) );
		}
		sr.stop();
	}
	{

	}

}

