#include <stock_data.h>
#include "small_buffer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "boost/shared_ptr.hpp"
#include <sstream>
#include "Trade.h"
#include "boost/thread.hpp"


typedef unsigned int UINT32;

using namespace std;
class processor{
	string& num;
	static size_t count;
	boost::mutex & outp_m;
	static boost::shared_ptr<ofstream> outp;

	const unsigned int TotalTypes;
	vector<size_t> numBytes; //used number of Bytes of idx type
	vector<size_t> messages;
	vector<size_t> totalBytes; //total used Bytes
	fstream in;
	ofstream out;
	bool fileErr;

public:
	processor(string& name, boost::mutex & mut):num (name), TotalTypes(100000), outp_m(mut)  // "YZ"
	{
	  if(outp == NULL)
	  {
		  stringstream output;
		  output << BINARY_DIR << "/output" << ".txt";
		  {
			  boost::mutex::scoped_lock lock(outp_m);
			  outp->open( output.str().c_str(), fstream::binary | fstream::out);
		  }
	  }

	  numBytes.resize(TotalTypes + 1, 0);
	  messages.resize(TotalTypes + 1, 0);
	  totalBytes.resize(TotalTypes + 1, 0);
		  

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

	  int n = 0; //total number of objects
	  UINT32 curTime = 0;
	  char* str = NULL;
	  bool toWrite = false;
	  Trade trade;
	  messages.assign(TotalTypes + 1, 0);
	  while (in)
	  {
		  if( in.peek() == EOF )
		  {
			  break;
		  }
		  trade.read(inp);
		  if(trade.time>curTime)
		  {
			  curTime = trade.time;
			  for(vector<size_t>::iterator it = numBytes.begin(), tb = totalBytes.begin(); it != numBytes.end(); it++, tb++)
			  {
				  *tb += *it;
			  }
			  numBytes.assign(TotalTypes + 1, 0);
		  }		
		  if ((trade.typ<TotalTypes)&&((numBytes[trade.typ] + trade.len + 3 * sizeof(UINT32)) <= 2048))
		  {
			  numBytes[trade.typ] += trade.len + 3 * sizeof(UINT32);
			  messages[trade.typ] ++;
		  }
		  str = new char[trade.len+1];
		  for (size_t i = 0; i < trade.len; i++)
		  {
			  in.read(&(str[i]), 1);
		  }
		  str[trade.len] = '\0';
		  n++;
		  if(str)
		  {
			  delete[] str;
		  }
		  if((trade.len == 0) || (trade.typ > TotalTypes) )
		  {
			  continue;
		  }
	  }
	  for(vector<size_t>::iterator it = numBytes.begin(), tb = totalBytes.begin(); it != numBytes.end(); ++it, ++tb)
	  {
		  *tb += *it;
	  }
	  int type = 0;

	  for(vector<size_t>::iterator tb = messages.begin(); tb != messages.end(); ++tb)
	  {
		  if (*tb == 0)
		  {
			  type++;
			  continue;
		  }
		  binary_reader::write_binary(*outp, type);
		  double val = (double)*tb/curTime;
		  binary_reader::write_binary(*outp, val);
		  type++;
	  }
	  inp.close();
	  outp->close();
	  return n;
	}
	void operator() ()
	{
	  process_data();
	}
};
boost::shared_ptr<ofstream> outp;
size_t processor::count = 0;
int main()
{
	binary_reader::stock_data( "", "", 1, 1, 1,1,1,1,1,1 );
}

