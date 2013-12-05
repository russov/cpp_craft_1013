#include "Stock_receiver.h"


stock_receiver::stock_receiver(void): c(config (data_path + string("config.ini")))
{
	c.get_trades();
	c.trade_threads();
	c.quote_threads();

	init_services( quote_services, c, true);	
	init_services(trade_services, c, false);

	init_listeners(true);
	init_listeners(false);

	int denom = c.get_trades().size();
	for(size_t i = 0; i < c.trade_threads(); i++ )
	{
		threads.create_thread(boost::bind(&stock_receiver::service_run, this, trade_services[i % denom]));

	}


}


stock_receiver::~stock_receiver(void)
{
	stop();
}

void stock_receiver::init_services( vector<shared_service> & vs, config & c, const bool quotes )
{
	size_t siz =
	quotes ? c.quote_threads() : c.trade_threads();
	vs.reserve(siz);
	for(size_t i = 0; i != siz; i++)
	{
		shared_service sp;
		sp.reset(new boost::asio::io_service);
		vs.push_back(sp);
	}
}

void stock_receiver::init_listeners( const bool quotes )
{
	listeners_vec  & lv = quotes ? quote_listeners :  trade_listeners;
	vector<shared_service> & vs = quotes ? quote_services : trade_services;
	size_t siz = quotes ? c.quote_ports() : c.trade_ports();
	addresses & a = quotes ? c.get_quotes() : c.get_trades();
	lv.reserve(a.size());
	for(size_t i = 0; i < siz; i++)
	{
		boost::shared_ptr<udp_listener> sp;
		sp.reset(new udp_listener(*vs[i], a[i].first, a[i].second));
		lv.push_back (sp);
	}
}

int stock_receiver::wait_some_data()
{
	for(size_t i = 0; i < trade_listeners.size(); ++i) { 
		if (trade_listeners[i]->messages().size() > 0 )
		{
			vector_messages msgs;
			boost::mutex::scoped_lock lock (trade_listeners[i]->protect_messages() );
			message::divide_messages(msgs, 
				trade_listeners[i]->messages_pop() , true);			
			return i;
		}
	}
	return -1;
}

void stock_receiver::service_run(shared_service serv)
{
	serv->run();
}

void stock_receiver::stop()
{
	vector<shared_service>::iterator it;
	for (it = quote_services.begin(); it != quote_services.end(); it++)
	{
		(*(it))->stop();
	}
	for (it = trade_services.begin(); it != trade_services.end(); it++)
	{
		(*(it))->stop();
	}
	threads.join_all();
}
