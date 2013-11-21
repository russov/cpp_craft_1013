#include "config_reader.h"

config_reader::config_reader(const std::string &config_file)
{ // Must read and parsing config_file

}

size_t config_reader::get_trade_thread_size() const
{
	return trade_thread_size_;
}

size_t config_reader::get_quote_thread_size() const
{
	return quote_thread_size_;
}

addresses_and_ports config_reader::get_trade_adr_and_ports() const
{
	return trade_ports_;
}

addresses_and_ports config_reader::get_quote_adr_and_ports() const
{
	return quote_ports_;
}

