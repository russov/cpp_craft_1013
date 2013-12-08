
#include <boost/cstdint.hpp>

namespace common_data
{
	struct quote_data
	{
		char security_symbol[16];
		boost::uint32_t time; // (значение времени в секундах начиная от 1900.01.01 00:00:00);
		double bid_price;
		double offer_price;
	};

	struct trade_data
	{
		char security_symbol[16];
		double price;
		boost::uint32_t time;
	};

	struct minute_datafeed
	{
		boost::uint32_t minute;// (значение времени в секундах начиная от 1900.01.01 00:00:00);
		char stock_name[16]; //(null-terminated);
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		double volume;
		double bid;
		double ask;
	};
}
