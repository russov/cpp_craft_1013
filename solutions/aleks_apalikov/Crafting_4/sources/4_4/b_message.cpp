#include "b_message.h"

task4_4::b_message::b_message( std::istream& inp )
{
	length_ = 0;
	if(!inp.eof())
		inp >> length_;
	if(length_ > 0)
	{
		char c;
		inp >> c; //space char 0x32
		content_ = new char[ length_ ];
	}
	inp.read( content_, length_ );
	if ( !inp )
		throw std::logic_error("bad input stream, a_message cannot be read");
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
	return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message()
{
	if(length_ > 0)
		delete [] content_;
}

const char task4_4::b_message::type() const
{
	return 'B';
}
const std::string task4_4::b_message::str() const
{
	std::string st(content_, length_);
	st = "b_message(" + length_ + '|' + st + ")";
	return st;
}
