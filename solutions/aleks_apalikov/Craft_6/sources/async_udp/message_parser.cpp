#include "message_parser.h"

int message::counter = 0;
message::message_category message::read_category()
{
	byte ch = 0;
	if( !inp_good )
		return message_category::end_reached;
	while (ch != delim::start)
	{
		if(inp.eof())
		{
			categ = message_category::end_reached;
			return categ;
		}
		get_byte(ch);
	}
	counter ++ ; // total delim:start 's
	place = 0;
	get_byte(ch);
	if((ch == message_category::bond)||(ch == message_category::equity)||(ch == message_category::equity))
	{
		categ = (message_category)ch;
		return categ;
	}
	else 
		return read_category();

}

message* message::read_next()
{
	if(read_category() == -1) return this;
	parse_rest();
	return this;
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


int quote::parse_rest()
{
	/*byte b;
	get_byte(b); 
	typ = (message_type) b;
	pair<char, int> p; 
	if(sec_len.find(b))
		get_string(security_symbol, header_len - place, sec_len[(char)b]);
*/
	return 0;
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
		get_string(security_symbol, header_len - place, i->second);
	else return -1;
	string st;
	const trade::trade_t* cur_trade;
	switch (i->first)
	{
	case message_type::short_trade:
		cur_trade = &get_short();
		break;
	case message_type::long_trade:
		cur_trade = &get_long();
		break;
	default:
		cout<<"Unknown type read"<<endl;
		return -1;
	};
	parse(cur_trade);
//	get_string(st, )

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
	get_char(denom);
	inp.seekg(cur_trade->pr_of + sec_symb, ios_base::beg);
	string s;
	get_string(s, 0, cur_trade->pr_len);
	price = boost::lexical_cast<double> (s);
	inp.seekg(cur_trade->vol_of + sec_symb, ios_base::beg);
	s.clear();
	get_string(s, 0, cur_trade->vol_len);
	volume = boost::lexical_cast<double> (s);
}
