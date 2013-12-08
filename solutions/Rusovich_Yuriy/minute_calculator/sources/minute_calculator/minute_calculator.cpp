#include <stdio.h>

#include "minute_calculator.h"

boost::mutex mtx_minute_datafeed;

minute_calculator::minute_calculator_process::minute_calculator_process()
{
	create_thread();
}

minute_calculator::minute_calculator_process::~minute_calculator_process()
{
	stop();
}

void minute_calculator::minute_calculator_process::create_thread()
{
	calculate_group.create_thread( boost::bind( &minute_calculator_process::quote_calculate, this ));
	calculate_group.create_thread( boost::bind( &minute_calculator_process::trade_calculate, this ));
}

void minute_calculator::minute_calculator_process::stop()
{
	calculate_group.interrupt_all();
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
			std::vector < common_data::quote_data >::iterator iterator_quote_data = quote_datas.begin();
			vector_minute_datafeed::iterator iterator;
			bool has_data = false;
			boost::mutex::scoped_lock lock( mtx_minute_datafeed );
			for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
			{
				if (std::strcmp((*iterator).stock_name, (*iterator_quote_data).security_symbol) == 0
					&& (*iterator).minute == (*iterator_quote_data).time / 60)
				{
					(*iterator).bid += (*iterator_quote_data).bid_price;
					(*iterator).ask += (*iterator_quote_data).offer_price;
					has_data = true;
					break;
				}
			}
			if (!has_data)
			{
				common_data::minute_datafeed datafeed;
				strcpy(datafeed.stock_name, (*iterator_quote_data).security_symbol);
				datafeed.minute = (*iterator_quote_data).time / 60;
				datafeed.bid = (*iterator_quote_data).bid_price;
				datafeed.ask = (*iterator_quote_data).offer_price;
				minute_datafeeds.push_back(datafeed);
			}
			quote_datas.erase(iterator_quote_data);
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
			std::vector < common_data::trade_data >::iterator iterator_trade_data = trade_datas.begin();
			vector_minute_datafeed::iterator iterator;
			bool has_data = false;
			boost::mutex::scoped_lock lock( mtx_minute_datafeed );
			for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
			{
				if (std::strcmp((*iterator).stock_name, (*iterator_trade_data).security_symbol) == 0
					&& (*iterator).minute == (*iterator_trade_data).time / 60)
				{
					if ((*iterator).open_price == 0 )
						(*iterator).open_price = (*iterator_trade_data).price;
					if ((*iterator).high_price < (*iterator_trade_data).price 
						|| (*iterator).high_price == 0 )
						(*iterator).high_price = (*iterator_trade_data).price;
					if ((*iterator).low_price > (*iterator_trade_data).price
						|| (*iterator).low_price == 0 )
						(*iterator).low_price = (*iterator_trade_data).price;
					(*iterator).close_price = (*iterator_trade_data).price;
					(*iterator).volume += (*iterator_trade_data).price;
					has_data = true;
					break;
				}
			}
			if (!has_data)
			{
				common_data::minute_datafeed datafeed;
				strcpy(datafeed.stock_name, (*iterator_trade_data).security_symbol);
				datafeed.minute = (*iterator_trade_data).time / 60;
				datafeed.open_price = (*iterator_trade_data).price;
				datafeed.close_price = (*iterator_trade_data).price;
				datafeed.high_price = (*iterator_trade_data).price;
				datafeed.low_price = (*iterator_trade_data).price;
				datafeed.volume = (*iterator_trade_data).price;
				minute_datafeeds.push_back(datafeed);
			}
			trade_datas.erase(iterator_trade_data);
		}
	}
}

void minute_calculator::minute_calculator_process::add_quote_data(const common_data::quote_data &quote)
{
	if ( quote.bid_price == 0 || quote.offer_price == 0 || std::strcmp(quote.security_symbol, "") == 0 )
	{
		throw std::logic_error("empty quote data");
		return;
	}
	quote_datas.push_back(quote);
}

void minute_calculator::minute_calculator_process::add_trade_data(const common_data::trade_data &trade)
{
	if ( trade.price == 0 || std::strcmp(trade.security_symbol, "") == 0 )
	{
		throw std::logic_error("empty trade data");
	}
	trade_datas.push_back(trade);
}

