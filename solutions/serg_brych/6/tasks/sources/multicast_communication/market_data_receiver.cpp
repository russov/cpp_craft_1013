#include "market_data_receiver.h"
#include "udp_listener.h"

multicast_communication::market_data_receiver::market_data_receiver(market_data_processor& processor):
	config_(new config_reader(config_file)), processor_(processor)
{
	if(config_->get_quote_adr_and_ports().size() > config_->get_quote_thread_size())
	{
		throw std::logic_error("Need more threads to process quote messages");
	}

	if(config_->get_trade_adr_and_ports().size() > config_->get_trade_thread_size())
	{
		throw std::logic_error("Need more threads to process trade messages");
	}
}

multicast_communication::market_data_receiver::~market_data_receiver()
{
	stop();
}

void multicast_communication::market_data_receiver::run()
{
	size_t count_of_multicast_addresses = config_->get_quote_adr_and_ports().size();
	if(count_of_multicast_addresses)
		for(size_t i = 0; i < config_->get_quote_thread_size(); i++)
		{
			quote_threads_.create_thread(
				boost::bind( &multicast_communication::market_data_receiver::quote_thread, this, config_->get_quote_adr_and_ports()[ i % count_of_multicast_addresses])
				);
		}

	count_of_multicast_addresses = config_->get_trade_adr_and_ports().size();
	if(count_of_multicast_addresses)
		for(size_t i = 0; i < config_->get_trade_thread_size(); i++)
		{
			trade_threads_.create_thread(
				boost::bind( &multicast_communication::market_data_receiver::trade_thread, this, config_->get_trade_adr_and_ports()[i % count_of_multicast_addresses])
				);
		}
}

void multicast_communication::market_data_receiver::stop()
{
	service_.stop();
	quote_threads_.join_all();
	trade_threads_.join_all();
}

void multicast_communication::market_data_receiver::quote_thread(multicast_communication::address_and_port &adr_port)
{
	async_udp::udp_listener(service_, adr_port.first, adr_port.second,[&](const std::string& data)
		{
			quote_message_list_ptr processed_messages;
			if(quote_message::parse_block(data,  processed_messages))
			{
				std::for_each(processed_messages.begin(), processed_messages.end(),[&](quote_message_ptr &msg)
					{
					processor_.new_quote(msg);
					}
				);
			}
		}
	);
	service_.run();
}

void multicast_communication::market_data_receiver::trade_thread(multicast_communication::address_and_port &adr_port)
{
	async_udp::udp_listener(service_, adr_port.first, adr_port.second,[&](const std::string& data)
		{
			trade_message_list_ptr processed_messages;
			if(trade_message::parse_block(data,  processed_messages))
			{
				std::for_each(processed_messages.begin(), processed_messages.end(),[&](trade_message_ptr &msg)
					{
					processor_.new_trade(msg);
					}
				);
			}
		}
	);
	service_.run();
}
