#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_

#include <vector>
#include <string>
#include <utility>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

#include "thread_safe_queue.h" 
#include "market_data_processor.h"
#include "udp_listener.h"

namespace multicast_communication 
{
    class market_data_receiver : virtual private boost::noncopyable
    {
        public: 

        explicit market_data_receiver( 
                size_t const trade_thread_size,
                size_t const quote_thread_size,
                std::vector<udp_listener::endpoint_addr> const& trade_ports,
                std::vector<udp_listener::endpoint_addr> const& quote_ports,
                market_data_processor&);

        ~market_data_receiver();

        void start(); 
        void stop(); 
        void join()
        { 
            t_group_.join_all();
        }
        size_t trades_size() const
        {
            return trade_queue_.size();
        }
        size_t quotes_size() const
        {
            return quote_queue_.size();
        }
        friend class udp_listener;
        private:
        boost::asio::io_service io_service_;

        thread_safe_queue< message_type > trade_queue_;
        mutable boost::mutex protect_trade_queue_;
        thread_safe_queue< message_type > quote_queue_;
        mutable boost::mutex protect_quote_queue_;

        boost::thread_group t_group_; 

        size_t const trade_thread_size_;
        size_t const quote_thread_size_;

        void process_();
        void process_();
        void receive(udp_listener::message const&);

        std::vector< udp_listener::udp_listener_ptr > listeners_; 

        market_data_processor& processor_; 

        bool stop_; 

    };
}
#endif
