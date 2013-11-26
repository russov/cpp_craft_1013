#include <stock_data.h>
#include "trades_filter.h"
#include "boost/thread.hpp"
#include <boost/lexical_cast.hpp>
#include <vector>

class processor{
	string* num;
public:
	processor(string& name)
	{
		num = new string(name); // "YZ"
	}
	~processor()
	{
		delete num;
	}
	int process_data()
	{
		stringstream input;
		stringstream output;
		if(num->size() == 1)
		{
			input << BINARY_DIR << "/input_00" << num << ".txt";
			output << BINARY_DIR << "/ouput_00" << num << ".txt";
		}
		else if (num->size() == 2 )
		{
			input << BINARY_DIR << "/input_0" << num << ".txt";
			output << BINARY_DIR << "/ouput_0" << num << ".txt";
		}
		else if (num->size() == 3)
		{
			input << BINARY_DIR << "/input_" << num << ".txt";
			output << BINARY_DIR << "/ouput_" << num << ".txt";
		}
		else {
			cout<< "Your changing part of name has wrong size"; 
			return -1;
		}
		ifstream inp( input.str().c_str(), fstream::binary | fstream::in);
		ofstream outp(output.str().c_str(), fstream::binary | fstream::out);
		size_t currentTime = 0;
		bool toWrite = false;
		Trade trade;
		while (!inp.eof())
		{
			if( trade.read(inp) == -1 )
				break;
			toWrite = (trade.typ>=MARKET_OPEN && trade.typ<=MARKET_CLOSE);
			if(currentTime != 0) //test of first read
			{
				if (trade.time <= currentTime-2)
				{
					toWrite |= false;
				}
				else 
				{
					toWrite |= true;
					currentTime = trade.time;
				}

			}
			else
			{
				toWrite |= true;
				currentTime = trade.time;
			}
			if(trade.len > 1000)
				break;
			if (toWrite)
			{
				trade.write(outp);
			}
			if( inp.peek() == EOF )
			{
				break;
			}
		}
		inp.close();
		outp.close();
		return 0;
	}
	void operator() ()
	{
		process_data();
	}
};
int main()
{
	size_t quantity = 10;
	vector<processor> process;
	boost::thread_group thr_gr;
	for(size_t i = 0; i < quantity; i++)
	{
		string st (boost::lexical_cast<string>(i));
		process.push_back(processor(st));
		thr_gr.create_thread(process[i]);
	}
	thr_gr.join_all();
}


int Trade::write( ofstream& o )
{
	binary_reader::write_binary(o, typ);
	binary_reader::write_binary(o, time);
	binary_reader::write_binary(o, len);
	string::iterator ch = str.begin();
	for(size_t i = 0; i < len; i++, ch++)
	{
		binary_reader::write_binary(o, *ch);
	}
	return 0;
}

int Trade::read( ifstream& inp )
{
	if( inp.peek() == EOF )
	{
		return -1;
	}
	binary_reader::read_binary(inp, typ);
	binary_reader::read_binary(inp, time);
	binary_reader::read_binary(inp, len);
	stringstream s;
	char ch = ' ';
	for(size_t i = 0; i < len; i++)
	{
		binary_reader::read_binary(inp, ch);
		s << ch;
	}
	str = s.str();
	return 0;
}
