#include "binary_datafeed.h"
int main()
{
	Datafeed d;
	d.run();
}

int Datafeed::run()
{
	if(fileErr)
		return -1;
	boost::thread_group thr_gr;
	boost::mutex tasks_mtx;
	bool finish = false;
	while(inp)
	{
		binary_reader::stock_data sd(inp);
		boost::shared_ptr<binary_reader::stock_data> p (& sd);
		map_stock::iterator it = tasks.find(sd.stock_name_);
		if(it != tasks.end())
		{
			{
				boost::mutex::scoped_lock lock (tasks_mtx);
				it->second.push(p);
			}
		}
		else
		{
			queue_of_pointers q;
			thr_gr.create_thread(processor(string(sd.stock_name_), tasks_mtx, q, finish));
			q.push(p);
			{
				boost::mutex::scoped_lock lock (tasks_mtx);
				tasks.insert( make_pair((string)sd.stock_name_, q));
			}
		}
		if (inp.peek() == EOF)
			break;
	}
	finish = true;
	thr_gr.join_all();
	return 0;
}

int processor::process_data()
{	
	stringstream input;	
	input << BINARY_DIR << "/ouput_" << num << ".txt";
	ofstream outp( input.str().c_str(), fstream::binary | fstream::out);
	if(!outp.is_open())
	{
		cout << "File not found! "<<endl;
		return 1;
	}

	int n = 0; //total number of objects
	while ( !(finish && que_.empty()) )
	{
		shared_stock sp;
		{
			boost::mutex::scoped_lock lock(que_mtx);
			if(!que_.empty())
			{
				sp = que_.front();
				que_.pop();
			}
		}
		sp->write(outp);
	}
	outp.close();
	return n;
}

