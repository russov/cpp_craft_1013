#include "config_reader.h"
#include <fstream>

multicast_communication::config_reader::config_reader(const std::string &config_file):trade_thread_size_(1),quote_thread_size_(1)
{ // Must read and parsing config_file
	std::ifstream config(config_file, std::fstream::in);
	if(config.is_open())
	{
		config >> trade_thread_size_;
		config >> quote_thread_size_;
		size_t count_of_multicasts;
		config >> count_of_multicasts;
		trade_adr_ports_.resize(count_of_multicasts);
		for(size_t i = 0; i < count_of_multicasts; i++)
		{
			config >> trade_adr_ports_[i].first >> trade_adr_ports_[i].second;
		}

		config >> count_of_multicasts;
		quote_adr_ports_.resize(count_of_multicasts);
		for(size_t i = 0; i < count_of_multicasts; i++)
		{
			config >> quote_adr_ports_[i].first >> quote_adr_ports_[i].second;
		}
	}
}

size_t multicast_communication::config_reader::get_trade_thread_size() const
{
	return trade_thread_size_;
}

size_t multicast_communication::config_reader::get_quote_thread_size() const
{
	return quote_thread_size_;
}

multicast_communication::addresses_and_ports multicast_communication::config_reader::get_trade_adr_and_ports() const
{
	return trade_adr_ports_;
}

multicast_communication::addresses_and_ports multicast_communication::config_reader::get_quote_adr_and_ports() const
{
	return quote_adr_ports_;
}

