#include "trade_message.h"

static const std::map< const char, double > denominator_map = boost::assign::map_list_of
    ('3' , 8.0) ('4' , 16.0) ('5' , 32.0) ('6' , 64.0) ('7' , 128.0) ('8' , 256.0)
    ('A' , 10.0) ('B' , 100.0) ('C' , 1000.0 ) ('D' , 10000.0 )
    ('E' , 100000.0 ) ('F' , 1000000.0 ) ('G' , 10000000.0  )
    ('H' , 100000000.0 )
	('I' , 1.0)
    ;


template<size_t size>
double get_number(std::istream& data)
{
    char buffer[ size ];
    data.read( buffer, size );
    return boost::lexical_cast< double >( buffer, size );
}

template<size_t size>
std::string get_string(std::istream& data)
{
    char buffer[ size ];
    data.read( buffer, size );
    return std::string( buffer , size );
}


multicast_communication::trade_message::trade_message():
	security_symbol_(""), price_(0.0), volume_(0.0), type_(trade_type::unknown_trade)
{
	
}

std::string multicast_communication::trade_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::trade_message::price() const
{
	return price_;
}

double multicast_communication::trade_message::volume() const
{
	return volume_;
}

multicast_communication::trade_message::trade_type multicast_communication::trade_message::type() const
{
	return type_;
}

bool multicast_communication::trade_message::parse_trade(std::istream &data)
{

	unsigned char msg_type;
	data.seekg(1, std::istream::cur);
	msg_type = data.get();

	switch(msg_type)
	{
	case trade_signature::sig_short_trade:
		parse_short_trade(data);
		type_ = trade_type::short_trade;
		return true;
	case trade_signature::sig_long_trade:
		parse_long_trade(data);
		type_ = trade_type::long_trade;
		return true;
	default:
		type_ = trade_type::unknown_trade;
		return false;
	}
	
}

void multicast_communication::trade_message::parse_short_trade(std::istream &data)
{
	data.seekg(st_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<st_security_symbol_len>(data);
		
	data.seekg(st_trade_volume_pos, std::istream::cur);
	
	volume_ = get_number<st_trade_volume_len>(data);

	double denominator = denominator_map.at(get_string<st_price_denominator_indicator_len>(data)[0]);
	price_ = get_number<st_trade_price_len>(data)/ denominator;	

	data.seekg(st_trade_end_pos, std::istream::cur);

}

void multicast_communication::trade_message::parse_long_trade(std::istream &data)
{
	data.seekg(lt_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<lt_security_symbol_len>(data);

	data.seekg(lt_price_denominator_indicator_pos, std::istream::cur);

	double denominator = denominator_map.at(get_string<lt_price_denominator_indicator_len>(data)[0]);
	price_ = get_number<lt_trade_price_len>(data)/ denominator;	
	
	volume_ = get_number<lt_trade_volume_len>(data);

	data.seekg(lt_trade_end_pos, std::istream::cur);
}