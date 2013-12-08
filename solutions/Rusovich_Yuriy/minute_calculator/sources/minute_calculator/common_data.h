
#include <boost/cstdint.hpp>

namespace common_data
{
	struct quote_data
	{
		char security_symbol[16];
		boost::uint32_t time; // (значение времени в секундах начиная от 1900.01.01 00:00:00);
		double bid_price;
		double offer_price;

		quote_data()
			: time(0), bid_price(0), offer_price(0)
		{}
	};

	struct trade_data
	{
		char security_symbol[16];
		double price;
		boost::uint32_t time;

		trade_data()
			: price(0), time(0)
		{}
	};

	struct minute_datafeed
	{
		boost::uint32_t minute; //значение времени в минутах;
		char stock_name[16]; //(null-terminated);
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		double volume;
		double bid;
		double ask;

		minute_datafeed()
			: minute(0), open_price(0), high_price(0), low_price(0),
			close_price(0), volume(0), bid(0), ask(0)
		{}
	};
}
