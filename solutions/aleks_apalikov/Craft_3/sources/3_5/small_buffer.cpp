#include "small_buffer.h"
#include <boost/lexical_cast.hpp>

boost::shared_ptr<ofstream> outp;
size_t processor::count = 0;
const UINT32 processor::TotalTypes = 10000; //100000
int main()
{
	size_t quantity = 3;
	vector<string*> nums;
	boost::thread_group thr_gr;
	boost::mutex total_mut;
	boost::mutex max_mut;
	vector<messages_t> total_msgs;
	total_msgs.resize(quantity);
	for(size_t j = 0; j < quantity; j++)
	{
		total_msgs[j].resize(processor::TotalTypes + 1, 0);
	}
	for(size_t i = 0; i < quantity; i++)
	{
		nums.push_back (new string(boost::lexical_cast<string>(i) ) );
		processor process (*nums[i], total_mut, total_msgs[i], max_mut);
		thr_gr.create_thread(process);
	}
	thr_gr.join_all();

}


int processor::process_data()
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
	UINT32 cur_time = 0;
	char* str = NULL;
	bool toWrite = false;
	Trade trade;
	messages.assign(TotalTypes + 1, 0);
	while (inp)
	{
		if( inp.peek() == EOF )
		{
			break;
		}
		trade.read(inp);
		if(trade.time>cur_time)
		{
			cur_time = trade.time;
			{
				boost::mutex::scoped_lock lock(max_mut);
				if(max_time < cur_time)
				{
					max_time = cur_time;
				}
			}
			{
				boost::mutex::scoped_lock lock(total_mut);
				for(vector<size_t>::iterator it = numBytes.begin(), tb = total_msgs.begin(); it != numBytes.end(); it++, tb++)
				{
					*tb += *it;
				}
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
			inp.read(&(str[i]), 1);
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
	{
		boost::mutex::scoped_lock lock (total_mut);
		for(vector<size_t>::iterator it = numBytes.begin(), tb = total_msgs.begin(); it != numBytes.end(); ++it, ++tb)
		{
			*tb += *it;
		}

	}
	inp.close();
	return n;
}
size_t processor::max_time = 0;