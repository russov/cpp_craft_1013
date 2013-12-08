#include <stdio.h>

#include "minute_calculator.h"

minute_calculator::minute_calculator_process::minute_calculator_process()
{
	create_thread();
}

minute_calculator::minute_calculator_process::~minute_calculator_process()
{
	calculate_group.join_all();
}

void minute_calculator::minute_calculator_process::create_thread()
{
	calculate_group.create_thread( boost::bind( &minute_calculator_process::quote_calculate, this ));
}

void minute_calculator::minute_calculator_process::quote_calculate()
{
	while(1)
	{
		if (quote_datas.empty())
		{
			boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
		} else
		{
			common_data::quote_data data = quote_datas.front();
			vector_minute_datafeed::iterator iterator;
			for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
			{
				if (std::strcmp((*iterator).stock_name, data.security_symbol) == 0
					&& (*iterator).minute / 60 == data.time / 60)
				{
					(*iterator).minute = data.time;
					(*iterator).bid += data.bid_price;
					(*iterator).ask += data.offer_price;
				} else
				{
					common_data::minute_datafeed datafeed;
					strcpy(datafeed.stock_name, data.security_symbol);
					datafeed.minute = data.time;
					datafeed.bid = data.bid_price;
					datafeed.ask = data.offer_price;
				}
			}
		}
	}
}

void minute_calculator::minute_calculator_process::trade_calculate()
{
	while(1)
	{
		if (trade_datas.empty())
		{
			boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
		} else
		{
			common_data::trade_data data = trade_datas.front();
			vector_minute_datafeed::iterator iterator;
			for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
			{
				if (std::strcmp((*iterator).stock_name, data.security_symbol) == 0
					&& (*iterator).minute / 60 == data.time / 60)
				{
					if ((*iterator).high_price < data.price)
						(*iterator).high_price = data.price;
					if ((*iterator).low_price > data.price)
						(*iterator).low_price = data.price;
					(*iterator).close_price = data.price;
					(*iterator).minute = data.time;
					++(*iterator).volume;
				} else
				{
					common_data::minute_datafeed datafeed;
					strcpy(datafeed.stock_name, data.security_symbol);
					datafeed.minute = data.time;
					datafeed.open_price = data.price;
					datafeed.close_price = data.price;
					datafeed.high_price = data.price;
					datafeed.low_price = data.price;
					datafeed.volume = 1;
				}
			}
		}
	}
}

void minute_calculator::minute_calculator_process::add_quote_data(const common_data::quote_data &quote)
{
	quote_datas.push_back(quote);
}

void minute_calculator::minute_calculator_process::add_trade_data(const common_data::trade_data &trade)
{
	trade_datas.push_back(trade);
}

