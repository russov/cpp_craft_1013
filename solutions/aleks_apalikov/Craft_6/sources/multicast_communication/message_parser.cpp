#include "message_parser.h"

int message::counter = 0;
message::message_category message::read_category()
{
	byte ch = 0;
	std::streampos pos = inp.tellg();//TODO: del; for tests
	if( !inp_good )
		return end_reached; // from enum message_category
/*	while (ch != start) // from enum delim
	{
		if(inp.eof())
		{
			categ = end_reached;
			return categ;
		}
		get_byte(ch);
	}*/
	counter ++ ; // total delim:start 's
	place = 0;
	get_byte(ch);
	if((ch == bond)||(ch == equity)||(ch == local_issue))
	{
		categ = (message_category)ch;
		return categ;
	}
	else 
		return read_category();

}

message* message::read_next()
{
	try{
	if(read_category() == -1) return this;
	parse_rest();
	return this;
	}
	catch( ... )
	{
		categ = end_reached;
		return this;
	}
}

void message::get_byte(byte & ch)
{
	if((inp.eof()) || (inp.peek() == EOF))
	{
		inp.setstate(iostream::eofbit);
		ch = EOF;
		return;
	}
	read_binary(inp, ch);
	place++;
}

void message::get_char( char & ch )
{
	if((inp.eof()) || (inp.peek() == EOF))
	{
		inp.setstate(iostream::eofbit);
		ch = EOF;
		return;
	}
	read_binary(inp, ch);
	place++;
}


void message::get_string( string & s, size_t pos, size_t len )
{
	for (size_t k = 0; k < pos; k++)
	{
		char c;
		get_char(c);
	}
	char * cs = new char [len + 1];
	for (size_t i = 0; i < len; i++)
	{
		get_char(cs[i]);
	}
	cs[len] = '\0';
	s = string(cs);
	len = s.length();
	delete [] cs;
}

int message::parse_rest()
{
	return 0;

}

void message::divide_messages( vector_messages vec_msgs, boost::shared_ptr<std::string> buffer, const bool quote )
{
	stringstream current_message;
	for(string::iterator it = buffer->begin(); it != buffer->end(); it++)
	{
		if(*it == delim::start ) //from enum delim
		{
			while( it != buffer->end())
			{
				current_message << *it;
				if((*it == delim::unit_separator) || (*it == delim::end))
				{
					boost::shared_ptr<message> sm((message *)new trade(current_message));
					vec_msgs.push_back(sm);
					vec_msgs.back()->read_next( );
					current_message.clear();
					continue;
				}
			}
		};
	}
}


int quote::parse_rest()
{
	if(inp.eof())
	{
		return -1;
	}
	byte b;
	get_byte(b); 
	typ = (message_type) b;
	map<char, int>::const_iterator i = sec_len.find(b);
	if((i != sec_len.end()) && b != 'I')
		get_string(security_symbol_, header_len - place, i->second);
	else return -1;
	string st;
	const quote::quote_t* cur_trade;
	switch (i->first)
	{
	case short_quote: //enum message_type::short_quote
		cur_trade = &get_short();
		break;
	case long_quote:
		cur_trade = &get_long();
		break;
	default:
		cout<<"Unknown type read"<<endl;
		return -1;
	};
	parse(cur_trade);

	return 0;
}

void quote::parse( const quote::quote_t* cur_trade )
{
	streamoff sec_symb = inp.tellg();
	inp.seekg(cur_trade->bid_denom_of + sec_symb, ios_base::beg);
	get_char(bid_denom_);
	denominator(bid_denom_); //TODO: del before pull req
	inp.seekg(cur_trade->bid_pr_of + sec_symb, ios_base::beg);
	string s;
	get_string(s, 0, cur_trade->bid_pr_len);
	bid_price_ = boost::lexical_cast<double> (s);
	inp.seekg(cur_trade->bid_vol_of + sec_symb, ios_base::beg);
	s.clear();
	get_string(s, 0, cur_trade->bid_vol_len);
	bid_volume_ = boost::lexical_cast<double> (s);
	inp.seekg(cur_trade->off_denom_of + sec_symb, ios_base::beg);
	get_char(offer_denom_);
	denominator(offer_denom_); //test
	inp.seekg(cur_trade->off_pr_of + sec_symb, ios_base::beg);
	s.clear();
	get_string(s, 0, cur_trade->off_pr_len);
	offer_price_ = boost::lexical_cast<double> (s);
	inp.seekg(cur_trade->off_vol_of + sec_symb, ios_base::beg);
	s.clear();
	get_string(s, 0, cur_trade->off_vol_len);
	offer_volume_ = boost::lexical_cast<double> (s);
}

// (char bvo, char bvl, char bpo, char bpl, char bdo, char ovo, char ovl, char opo, char opl, char odo) //starting from end of security symbol
const vector<quote::quote_t> quote::short_long = boost::assign::list_of (quote_t (12, 3, 4, 8, 3, 25, 3, 17, 8, 16 ) )
(quote_t ( 29, 7, 17, 12, 16, 49, 7, 37, 12, 36 ) );

const quote::quote_t& quote::get_short()
{
	return short_long[0];
}
const quote::quote_t& quote::get_long()
{
	return short_long[1];
}

int trade::parse_rest()
{
	if(inp.eof())
	{
		return -1;
	}
	byte b;
	get_byte(b); 
	typ = (message_type) b;
	map<char, int>::const_iterator i = sec_len.find(b);
	if((i != sec_len.end()) && b != 'D')
		get_string(security_symbol_, header_len - place, i->second);
	else return -1;
	string st;
	const trade::trade_t* cur_trade;
	switch (i->first)
	{
	case short_trade: //enum message_type::short_trade
		cur_trade = &get_short();
		break;
	case long_trade:
		cur_trade = &get_long();
		break;
	default:
		cout<<"Unknown type read"<<endl;
		return -1;
	};
	parse(cur_trade);

	return 0;
}



vector<trade::trade_t> trade::short_long = boost::assign::list_of(trade_t(1, 4, 6, 8, 5)) (trade_t(34, 9, 22, 12, 21));

const trade::trade_t& trade::get_short()
{
	return short_long[0];
}
const trade::trade_t& trade::get_long()
{
	return short_long[1];
}

void trade::parse(const trade::trade_t* cur_trade )
{
	streamoff sec_symb = inp.tellg();
	inp.seekg(cur_trade->denom_of + sec_symb, ios_base::beg);
	get_char(denom_);
	denominator(denom_); //TODO: del before pull req
	inp.seekg(cur_trade->pr_of + sec_symb, ios_base::beg);
	string s;
	get_string(s, 0, cur_trade->pr_len);
	price_ = boost::lexical_cast<double> (s);
	inp.seekg(cur_trade->vol_of + sec_symb, ios_base::beg);
	s.clear();
	get_string(s, 0, cur_trade->vol_len);
	volume_ = boost::lexical_cast<double> (s);
}
