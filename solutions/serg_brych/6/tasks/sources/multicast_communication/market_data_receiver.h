#ifndef _MARKET_DATA_RECEIVER_H_
#define _MARKET_DATA_RECEIVER_H_

#include "market_data_processor.h"
#include "config_reader.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include <string>

static const std::string config_file("config.ini");

namespace multicast_communication
{
	class market_data_receiver : virtual private boost::noncopyable
	{
	private:
		boost::scoped_ptr<config_reader> config_;
		boost::thread_group	quote_threads_;
		boost::thread_group	trade_threads_;
	public:
		market_data_receiver():config_(new config_reader(config_file))
		{
			
		};
		
		void run();
		void stop();

	};

}


#endif _MARKET_DATA_RECEIVER_H_