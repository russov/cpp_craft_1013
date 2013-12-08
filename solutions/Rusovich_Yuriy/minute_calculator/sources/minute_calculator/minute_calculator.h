#ifndef _MINUTE_CALCULATOR_PROCESSOR_H_
#define _MINUTE_CALCULATOR_PROCESSOR_H_

#include <boost/thread.hpp>
#include <vector>
#include <map>

#include "common_data.h"

namespace minute_calculator
{
	class minute_calculator_process
	{
	public:
		explicit minute_calculator_process();
		~minute_calculator_process();

		void add_quote_data(const common_data::quote_data &quote);
		void add_trade_data(const common_data::trade_data &trade);
		
		void stop();
	
	protected:
		void create_thread();

		

		void quote_calculate();
		void trade_calculate();

		boost::thread_group calculate_group;
		std::vector < common_data::quote_data > quote_datas;
		std::vector < common_data::trade_data > trade_datas;

		typedef std::vector < common_data::minute_datafeed > vector_minute_datafeed;
		vector_minute_datafeed minute_datafeeds;

	public:
		inline minute_calculator::minute_calculator_process::vector_minute_datafeed get_minute_datafeed()
		{
			return minute_datafeeds;
		}
	};
}

#endif //_MINUTE_CALCULATOR_PROCESSOR_H_