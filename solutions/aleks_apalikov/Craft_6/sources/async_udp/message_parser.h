#ifndef _MESSAGE_PARSER_H_
#define _MESSAGE_PARSER_H_
#include <fstream>
#include <iostream>

typedef unsigned char byte;
using namespace std;
enum delim
{
	start = 0x01,
	unit_separator = 0x1F,
	end = 0x03
};

template< class T >
void read_binary( std::ifstream& in, T& t, const size_t len = sizeof( T ) )
{
	in.read( reinterpret_cast< char* >( &t ), len );
}
template< class T >
void write_binary( std::ofstream& out, T& t, const size_t len = sizeof( T ) )
{
	out.write( reinterpret_cast< const char* >( &t ), len );
}
class message
{
private:
	ifstream& inp;
	bool inp_good;
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
public:
	static int counter;
	message(ifstream& inf): inp(inf)
	{
		if (!inf.is_open())
		{
			cout<< "Error in binary file! "<<endl;
			inp_good = false;
		}
		inp_good = true;

	}
	message_category read_category(); //search for listed above message_category
	message read_next();
	void get_byte(byte & b);
	virtual int parse_rest();
};
class quote: public message
{
	virtual int parse_rest();

};
class trade:public message
{
	virtual int parse_rest();

};

#endif