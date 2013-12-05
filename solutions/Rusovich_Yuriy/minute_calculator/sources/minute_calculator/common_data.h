
#include <boost/cstdint.hpp>

namespace common_data
{
	struct quote_data
	{
		char[16] security_symbol;
		double price;
		boost::uint32_t time;
		double bid_price;
		double offer_price;
	}

	struct trade_data
	{
		char[16] security_symbol;
		double price;
		boost::uint32_t time;
	}

	struct minute_datafeed
	{
		boost::uint32_t minute;// (значение времени в секундах начиная от 1900.01.01 00:00:00);
		char [16] stock_name; //(null-terminated);
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		double volume;
		double bid;
		double ask;
	};
}
