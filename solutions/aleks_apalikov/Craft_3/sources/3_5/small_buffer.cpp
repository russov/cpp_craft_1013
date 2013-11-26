#include <stock_data.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "boost/shared_ptr.hpp"
#include <sstream>
#include "Trade.h"

using namespace std;
class processor{
	string& num;
	static size_t count;
	static boost::shared_ptr<ofstream> outp;

public:
	processor(string& name):num (name)  // "YZ"
	{
	  if(outp == NULL)
	  {
		  stringstream output;
		  output << BINARY_DIR << "/output" << ".txt";
		  outp->open( output.str().c_str(), fstream::binary | fstream::out);
	  }
		  

	}
	~processor()
	{
		if(count == 0)
		{
			outp->close();
		}
	}
	int process_data()
	{
	  stringstream input;
	  if(num.size() == 1)
	  {
		  input << BINARY_DIR << "/input_00" << num << ".txt";
	  }
	  else if (num.size() == 2 )
	  {
		  input << BINARY_DIR << "/input_0" << num << ".txt";
	  }
	  else if (num.size() == 3)
	  {
		  input << BINARY_DIR << "/input_" << num << ".txt";
	  }
	  else {
		  cout<< "Your changing part of name has wrong size"; 
		  return -1;
	  }
	  ifstream inp( input.str().c_str(), fstream::binary | fstream::in);
	  if(!inp.is_open() || !outp->is_open())
	  {
		  cout << "File not found! "<<endl;
		  return 1;
	  }

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
			  trade.write(*outp);
		  }
		  if( inp.peek() == EOF )
		  {
			  break;
		  }
	  }
	  inp.close();
	  outp->close();
	  return 0;
	}
	void operator() ()
	{
	  process_data();
	}
};
boost::shared_ptr<ofstream> outp;
int main()
{
	binary_reader::stock_data( "", "", 1, 1, 1,1,1,1,1,1 );
}

