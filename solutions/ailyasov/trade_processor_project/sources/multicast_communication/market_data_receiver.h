#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_

#include <vector>
#include <string>
#include <utility>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

#include "thread_safe_queue.h" 
#include "market_data_processor.h"

namespace multicast_communication 
{
    class market_data_receiver : virtual private boost::noncopyable
    {
        class conn;
        public: 
        typedef std::vector<char> message_type; typedef boost::shared_ptr< message_type > buffer_type;
        typedef std::pair<std::string, unsigned short> endpoint_addr;
        typedef boost::shared_ptr< boost::asio::ip::udp::endpoint > endpoint_ptr;
        typedef boost::shared_ptr< boost::asio::ip::udp::socket > socket_ptr;
        typedef boost::shared_ptr< conn > conn_ptr;

        explicit market_data_receiver( 
                size_t const trade_thread_size,
                size_t const quote_thread_size,
                std::vector<endpoint_addr> const& trade_ports,
                std::vector<endpoint_addr> const& quote_ports,
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
        friend class conn;
        private:
        boost::asio::io_service io_service_;

        thread_safe_queue< message_type > trade_queue_;
        mutable boost::mutex protect_trade_queue_;
        thread_safe_queue< message_type > quote_queue_;
        mutable boost::mutex protect_quote_queue_;

        boost::thread_group t_group_; 

        static const size_t default_buffer_size; 

        size_t const trade_thread_size_;
        size_t const quote_thread_size_;

        static void convert_block_to_trades(message_type const&, std::vector< trade_message_ptr >& );
        static void convert_block_to_quotes(message_type const&, std::vector< quote_message_ptr >& );
        static void split_block(message_type const&, std::vector<message_type>&);
        static void convert_messages_to_trades(std::vector<message_type> const&, std::vector< trade_message_ptr >& );
        static void convert_messages_to_quotes(std::vector<message_type> const&, std::vector< quote_message_ptr >& );

        void process_trades_();
        void process_quotes_();

        std::vector< conn_ptr > v_conn_; 

        market_data_processor& processor_; 

        bool stop_;

        class conn: virtual private boost::noncopyable
        {
            public:
                conn(market_data_receiver& receiver
                        , endpoint_addr addr
                        , bool trade_type);

                void register_listen_( buffer_type pre_buffer = buffer_type(), const size_t previous_size = 0 );
                void listen_handler_( buffer_type bt, const boost::system::error_code& error, const size_t bytes_received );
                void socket_reload_();
            private: 
                market_data_receiver& receiver_;
                endpoint_addr addr_;
                bool trade_type_;
                socket_ptr socket_;
                endpoint_ptr endpoint_;
        };
    };
}
#endif
