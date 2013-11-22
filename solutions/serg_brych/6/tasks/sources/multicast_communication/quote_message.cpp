#include "quote_message.h"


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

multicast_communication::quote_message::quote_message():
	security_symbol_(""), bid_price_(0.0), bid_volume_(0.0),offer_price_(0.0), offer_volume_(0.0)
{

}

bool multicast_communication::quote_message::parse_quote(std::istream &data)
{
	unsigned char msg_type;
	data.seekg(1, std::istream::cur);
	msg_type = data.get();

	switch(msg_type)
	{
	case 'D':
		parse_short_quote(data);
		type_ = quote_type::short_quote;
		return true;
	case 'B':
		parse_long_quote(data);
		type_ = quote_type::long_quote;
		return true;
	default:
		type_ = quote_type::unknown_quote;
		return false;
	}
}

void multicast_communication::quote_message::parse_short_quote(std::istream &data)
{
	data.seekg(sq_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<sq_security_symbol_len>(data);
		
	data.seekg(sq_bid_price_denominator_indicator_pos, std::istream::cur);
		
	double bid_price_denominator = denominator_map.at(get_string<sq_denominato_len>(data)[0]);
	bid_price_ = get_number<sq_price_len>(data)/ bid_price_denominator;
	bid_volume_ = get_number<sq_volume_len>(data);

	data.seekg(sq_offer_price_denominator_indicator_pos, std::istream::cur);
		
	double offer_price_denominator = denominator_map.at(get_string<sq_denominato_len>(data)[0]);
	offer_price_ = get_number<sq_price_len>(data)/ offer_price_denominator;
	offer_volume_ = get_number<sq_volume_len>(data);

	data.seekg(sq_short_quote_end_pos, std::istream::cur);
}

void multicast_communication::quote_message::parse_long_quote(std::istream &data)
{
	data.seekg(lq_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<lq_security_symbol_len>(data);

	data.seekg(lq_bid_price_denominator_indicator_pos, std::istream::cur);
		
	double bid_price_denominator = denominator_map.at(get_string<lq_denominato_len>(data)[0]);
	bid_price_ = get_number<lq_price_len>(data)/ bid_price_denominator;
	bid_volume_ = get_number<lq_volume_len>(data);

	data.seekg(lq_offer_price_denominator_indicator_pos, std::istream::cur);
		
	double offer_price_denominator = denominator_map.at(get_string<lq_denominato_len>(data)[0]);
	offer_price_ = get_number<lq_price_len>(data)/ offer_price_denominator;
	offer_volume_ = get_number<lq_volume_len>(data);

	data.seekg(lq_short_quote_end_pos, std::istream::cur);
}

std::string multicast_communication::quote_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::quote_message::bid_price() const
{
	return bid_price_;
}

double multicast_communication::quote_message::bid_volume() const
{
	return bid_volume_;
}

double multicast_communication::quote_message::offer_price() const
{
	return offer_price_;
}

double multicast_communication::quote_message::offer_volume() const
{
	return offer_volume_;
}

multicast_communication::quote_message::quote_type multicast_communication::quote_message::type() const
{
	return type_;
}