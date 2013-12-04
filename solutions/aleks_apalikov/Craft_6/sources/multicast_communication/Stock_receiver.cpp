#include "Stock_receiver.h"


stock_receiver::stock_receiver(void): c(config (data_path + string("config.ini")))
{
	c.get_trades();
	c.trade_threads();
	c.quote_threads();

	quote_services.reserve(c.quote_threads());
	for(size_t i = 0; i != c.quote_threads(); i++)
	{
		shared_service sp;
		sp.reset(new boost::asio::io_service);
		quote_services.push_back(sp);
	}
	for(size_t i = 0; i != c.quote_threads(); i++)
	{
		cout << quote_services[i].use_count() << endl;
	}
	quote_listeners.reserve(c.quote_ports());
	for(listeners_vec::iterator i = quote_listeners.begin(); i != quote_listeners.end(); i++)
	{
		boost::shared_ptr<void> sp ;
		i->reset (new udp_listener(boost::asio::io_service(), 
			"212.12.23.32", 3128));
	}

}


stock_receiver::~stock_receiver(void)
{
}