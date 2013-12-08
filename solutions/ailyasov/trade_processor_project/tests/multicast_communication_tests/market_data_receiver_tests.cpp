#include <vector>
#include <utility>
#include <fstream>

//#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "test_registrator.h"
#include "market_data_receiver.h"
#include "market_data_processor.h"
#include "udp_listener.h"

namespace multicast_communication
{
    namespace tests_
    {

		class market_data_processor_test_helper : public market_data_processor
		{
		public:
			explicit market_data_processor_test_helper() {}
			virtual ~market_data_processor_test_helper() {}
		private:
            thread_safe_queue< trade_message_ptr > trades_;
            thread_safe_queue< quote_message_ptr > quotes_;
		};

        struct market_data_receiver_test_helper : public market_data_receiver
        {
            public:
                market_data_receiver_test_helper( 
                        size_t const trade_thread_size,
                        size_t const quote_thread_size,
                        std::vector<udp_listener::endpoint_addr> const& trade_ports,
                        std::vector<udp_listener::endpoint_addr> const& quote_ports,
                        market_data_processor& p) 
                    : market_data_receiver(trade_thread_size, quote_thread_size,
                            trade_ports, quote_ports, p)
                {
                } 
        };

        void read_from(std::string const& file, std::vector<message_type>& messages)
        {
            message_type m; std::ifstream in(file, std::ios::in | std::ios::binary );
            while(in.good())
            {
                char c = static_cast<char>(in.get());
                if(c == 0x01)
                {
                    m.clear();
                }
                m.push_back(c);
                if(c == 0x03)
                {
                    messages.push_back(m);
                }
            }
        } 

        unsigned int read_and_send(boost::asio::io_service& service,
                std::vector<std::pair<std::string, unsigned short> >& ports) 
        {
            unsigned int sent_total = 0u;
            for(std::vector<std::pair<std::string, unsigned short> >::iterator it = ports.begin();
                    it != ports.end(); ++it)
            {
                std::vector<message_type> messages;
                read_from(SOURCE_DIR "/tests/data/" + it->first + ".udp", messages);
            }
            return sent_total;
        } 

        void market_data_receiver_tests()
        {
            {
                std::string address("224.0.0.0");
                market_data_processor processor;
                std::vector<std::pair<std::string, unsigned short> > trades;
                std::vector<std::pair<std::string, unsigned short> > quotes;
                market_data_receiver receiver( 1, 1, trades, quotes, processor ) ;
            }

            {
                std::vector<std::pair<std::string, unsigned short> > t_addr;
                std::vector<std::pair<std::string, unsigned short> > q_addr;

                q_addr.push_back( std::make_pair("233.200.79.0", 61000) );
                q_addr.push_back( std::make_pair("233.200.79.1", 61001) );
                q_addr.push_back( std::make_pair("233.200.79.2", 61002) );
                q_addr.push_back( std::make_pair("233.200.79.3", 61003) );
                q_addr.push_back( std::make_pair("233.200.79.4", 61004) );
                q_addr.push_back( std::make_pair("233.200.79.5", 61005) );
                q_addr.push_back( std::make_pair("233.200.79.6", 61006) );
                q_addr.push_back( std::make_pair("233.200.79.7", 61007) );
                t_addr.push_back( std::make_pair("233.200.79.128", 62128) );
                t_addr.push_back( std::make_pair("233.200.79.129", 62129) );
                t_addr.push_back( std::make_pair("233.200.79.130", 62130) );
                t_addr.push_back( std::make_pair("233.200.79.131", 62131) );
                t_addr.push_back( std::make_pair("233.200.79.132", 62132) );
                t_addr.push_back( std::make_pair("233.200.79.133", 62133) );
                t_addr.push_back( std::make_pair("233.200.79.134", 62134) );
                t_addr.push_back( std::make_pair("233.200.79.135", 62135) );

                market_data_processor_test_helper processor;
                market_data_receiver_test_helper receiver(2, 2, t_addr, q_addr, processor );

                //boost::thread run(service_run, boost::ref( service ) );

                //receiver.start();

                //unsigned int quotes_sent = read_and_send(service, q_addr);
                //unsigned int trades_sent = read_and_send(service, t_addr); 

                //int attempts = 0;

                //while(processor.trades().size() != trades_sent && attempts++ < 10)
                //{
                //    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                //}

                //attempts = 0;

                //while(processor.quotes().size() != quotes_sent && attempts++ < 10)
                //{
                //    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                //}

                //run.join();
                //BOOST_CHECK_EQUAL( receiver.trades_size(), 0 );
                //BOOST_CHECK_EQUAL( receiver.quotes_size(), 0 );

                //BOOST_CHECK_EQUAL( processor.trades().size(), 4000 );
                //BOOST_CHECK_EQUAL( processor.quotes().size(), 4195 );
            }
        }
    }
} 
