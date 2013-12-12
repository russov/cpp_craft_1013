#include "test_registrator.h"
#include <string>
#include "Stock_receiver.h"
#include "config.h"
#include <sstream>
#include "market_data_processor.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>

typedef vector<ifstream*> vec_ifstr;
typedef vector<ifstream*>::iterator f_iter;
typedef boost::asio::ip::udp::endpoint endpoint;
typedef vector<endpoint>::iterator e_iter;
bool all_empty(vec_ifstr & tr, vec_ifstr & q)
{
	bool b = true;
	for (f_iter f = tr.begin(); f != tr.end(); f++)
	{
		b &= ((*f)->peek() == EOF);
	}
	for (f_iter f = q.begin(); f != q.end(); f++)
	{
		b &= ((*f)->peek() == EOF);
	}
	return b;
}

void clear_files(vec_ifstr & tr, vec_ifstr & q)
{
	for (f_iter f = tr.begin(); f != tr.end(); f++)
	{
		delete *f;
	}
	for (f_iter f = q.begin(); f != q.end(); f++)
	{
		delete *f;
	}
}
void async_udp::receiver_test()
{
	typedef boost::asio::ip::address address;
	{
		stock_receiver sr("results2.txt");
		boost::asio::io_service service;
		boost::asio::ip::udp::endpoint endp( address::from_string( "233.200.79.0" ), 61000 );
		boost::asio::ip::udp::socket socket( service, endp.protocol() );

		stringstream ss;
		ifstream fs( string (data_path + "stock_receiver_test.dat") .c_str() );
		copy(istreambuf_iterator<char>(fs), istreambuf_iterator<char>(), 
			ostreambuf_iterator<char>(ss));
		string str = ss.str();
		socket.send_to( boost::asio::buffer( str.c_str(), str.size() ), endp );
		while( sr.wait_some_data() == -1 )
		{
			socket.send_to( boost::asio::buffer( str.c_str(), str.size() ), endp );
			boost::this_thread::sleep_for( boost::chrono::nanoseconds( 100 ) );
		}
		sr.stop();
	}
	{
		cout << data_path << endl;
		stock_receiver sr;
		boost::asio::io_service service;
		boost::asio::ip::udp::endpoint endp( address::from_string( "233.200.79.128" ), 62128 ); 
		boost::asio::ip::udp::socket socket( service, endp.protocol() );
		config c(data_path + string("config.ini"));
		typedef addresses::const_iterator iter;
		using namespace boost::asio::ip;

		vector<endpoint> trades;
		vec_ifstr trade_files;
		for (iter i = c.get_trades().begin(); i != c.get_trades().end(); ++i )
		{
			trades.push_back( endpoint(address::from_string( i->first ), i->second ));
			trade_files.push_back(new ifstream (string(data_path + i->first + ".udp").c_str()) );
		}
		vector<endpoint> quotes;
		vec_ifstr quote_files;
		for (iter q = c.get_quotes().begin(); q != c.get_quotes().end(); ++q )
		{
			quotes.push_back( endpoint(address::from_string( q->first ), q->second ));
			quote_files.push_back(new ifstream ( string(data_path + q->first + ".udp").c_str()) );
		}
	
		while (!all_empty(quote_files, trade_files))
		{
			f_iter tf = trade_files.begin();
			for (e_iter t = trades.begin(); t != trades.end(); t++, tf++ )
			{
				stringstream ss;
				if((*tf)->peek() != EOF)
				{
					message::read_block(ss, **tf);
					socket.send_to( boost::asio::buffer( ss.str().c_str(), ss.str().size() ), *t );
					ss.str("");
				}				
			}
			f_iter qf = quote_files.begin();
			for (e_iter q = quotes.begin(); q != quotes.end(); q++, qf++ )
			{
				stringstream ss;
				if((*qf)->peek() != EOF)
				{		
					message::read_block(ss, **qf);
					socket.send_to( boost::asio::buffer( ss.str().c_str(), ss.str().size() ), *q );
					ss.str("");
				}	
			}
			boost::this_thread::sleep_for( boost::chrono::nanoseconds( 100 ) );
			sr.wait_some_data();
		}

		clear_files(trade_files, quote_files);
		cout << "Total messages was parsed and passed: " << message::count << endl; 

		while( sr.wait_some_data() != -1 )
		{
			boost::this_thread::sleep_for( boost::chrono::nanoseconds( 1 ) );
		}
	}

}

