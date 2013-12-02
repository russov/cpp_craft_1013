#include "message_parser.h"

int message::counter = 0;
message::message_category message::read_category()
{
	byte ch = 0;
	while (ch != delim::start)
	{
		if(!inp.is_open())
			return message_category::end_reached;
		get_byte(ch);
		counter ++ ;
	}
	get_byte(ch);
	if((ch == message_category::bond)||(ch == message_category::equity)||(ch == message_category::equity))
	{
		categ = (message_category)ch;
		return categ;
	}
	else 
		return read_category();

}

message message::read_next()
{
	read_category();
	parse_rest();
	return message(inp);
}

void message::get_byte(byte & ch)
{
	if(inp.peek() == EOF)
	{
		inp.close();
		ch = EOF;
		return;
	}
	read_binary(inp, ch);
}

int message::parse_rest()
{
	return 0;

}


int quote::parse_rest()
{

	return 0;


}

int trade::parse_rest()
{

	return 0;
}
