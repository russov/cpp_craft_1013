#ifndef _TRADES_FILTER_
#define _TRADES_FILTER_
#include <iostream>
#include <fstream>
using namespace std;
enum Types {
	MARKET_OPEN = 1u,
	TRADE = 2u,
	QUOTE = 3u,
	MARKET_CLOSE = 4u
};
struct Trade
{
	size_t typ;
	size_t time;
	size_t len;
	string str;
	Trade(size_t t = 0, size_t time = 0, size_t l = 0, char * st = ""): typ(t), time(time), len(l)
	{
		str = string(st);
	}
	void change(size_t t = 0, size_t time = 0, size_t l = 0, char * st = "")
	{
		typ = t;
		time = time;
		len = l;
		str = string(st);
	}
	int write(ofstream& o);
	int read(ifstream& i);
	
};


#endif // _TRADES_FILTER_
