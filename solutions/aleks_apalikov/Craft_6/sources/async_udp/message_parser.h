#ifndef _MESSAGE_PARSER_H_
#define _MESSAGE_PARSER_H_
#include <fstream>
#include <iostream>
#include <map>
#include "boost/assign.hpp"
#include "boost/lexical_cast.hpp"

typedef unsigned char byte;
using namespace std;
enum delim
{
	start = 0x01,
	unit_separator = 0x1F,
	end = 0x03
};

template< class T >
void read_binary( std::istream& in, T& t, const size_t len = sizeof( T ) )
{
	in.read( reinterpret_cast< char* >( &t ), len );
}
template< class T >
void write_binary( std::ostream& out, T& t, const size_t len = sizeof( T ) )
{
	out.write( reinterpret_cast< const char* >( &t ), len );
}
class message
{
protected:
	istream& inp;
	size_t place; //where are we reading
	bool inp_good;
	string security_symbol;
	enum message_category
	{
		bond = 'B',
		equity = 'E',
		local_issue = 'L',
		end_reached = -1
	};
	message_category categ;
	enum message_type
	{
		long_quote = 'B',
		short_quote = 'D',
		long_trade = 'B',
		short_trade = 'I'
	};
	enum {
		header_len = 24 //security_symbol start
	};

	message_type typ;
public:
	message_type get_type()
	{
		return typ;
	}
	static int counter;
	message(): inp(cin) // not neccessary construct
	{
	}
	message(istream& inf): inp(inf)
	{
		if (inf.eof())
		{
			cout<< "Error in stringstream: it is empty "<<endl;
			inp_good = false;
			categ = message_category::end_reached;
			return;
		}
		inp_good = true;
		place = 0;
		inp.seekg(place);

	}
	message_category read_category(); //search for listed above message_category
	message* read_next();
	void get_byte(byte & b);
	void get_char(char & c);
	message_category get_categ()
	{
		return categ;
	}
	void get_string(string & s, size_t pos, size_t len);
	virtual int parse_rest();
};
class quote: public message
{
public:
	char bid_denom; //TODO: make getters for tests or struct for tests
	double bid_price;
	double bid_volume;
	char offer_denom;
	double offer_price;
	double offer_volume;
	struct quote_t //offset of bytes after sec_symb
	{
		char bid_vol_of;
		char bid_vol_len;
		char bid_pr_of;
		char bid_pr_len;
		char bid_denom_of;
		char off_vol_of;
		char off_vol_len;
		char off_pr_of;
		char off_pr_len;
		char off_denom_of;
		quote_t(char bvo, char bvl, char bpo, char bpl, char bdo, char ovo, char ovl, char opo, char opl, char odo): 
			bid_vol_of(bvo), bid_vol_len(bvl), bid_pr_of(bpo), bid_pr_len(bpl), bid_denom_of(bdo),
			off_vol_of(ovo), off_vol_len(ovl), off_pr_of(opo), off_pr_len(opl), off_denom_of(odo)
		{
		}
	} ;
private:
	virtual int parse_rest();
	void parse( const quote::quote_t* cur_trade );
	static const vector<quote_t> short_long;
public:
	const static quote_t& get_short();
	const static quote_t& get_long();
public:
	quote(istream& ifs): message(ifs)
	{}
};

class trade:public message
{
public:
	struct trade_t //offset of bytes after sec_symb
	{
		char vol_of;
		char vol_len;
		char pr_of;
		char pr_len;
		char denom_of;
		trade_t(char vo, char vl, char po, char pl, char d_o): vol_of(vo), vol_len(vl), pr_of(po), pr_len(pl), denom_of(d_o)
		{
		}
	} ;
	char denom;
	double price;
	double volume;
	static vector<trade_t> short_long;
	const static trade_t& get_short();
	const static trade_t& get_long();
	trade(istream& ifs): message(ifs)
	{
	}
	virtual int parse_rest();
	void parse(const trade::trade_t* cur_trade );
};
static const map<char, int> sec_len = boost::assign::map_list_of('D', 3) ('B', 11) ('I', 3);
#endif