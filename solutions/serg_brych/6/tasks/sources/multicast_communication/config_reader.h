#ifndef __CONFIG_READER_H__
#define __CONFIG_READER_H__

#include <vector>
#include <string>
#include <boost/noncopyable.hpp>


typedef std::vector<std::string, unsigned short> addresses_and_ports;


class config_reader : virtual protected boost::noncopyable
{
public:
	config_reader(const std::string &);
	size_t get_trade_thread_size() const;
	size_t get_quote_thread_size() const;
	addresses_and_ports get_trade_adr_and_ports() const;
	addresses_and_ports get_quote_adr_and_ports() const;
private:
	size_t trade_thread_size_;
	size_t quote_thread_size_;
	addresses_and_ports trade_ports_;
	addresses_and_ports quote_ports_;

};

#endif __CONFIG_READER_H__