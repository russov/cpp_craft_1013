#include <vector>
#include <string>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include "market_data_receiver.h"
#include "market_data_processor.h"
#include "parser.h"

namespace multicast_communication
{
    const size_t market_data_receiver::default_buffer_size = 1000ul;

    market_data_receiver::market_data_receiver(
            size_t const trade_thread_size,
            size_t const quote_thread_size,
            std::vector<endpoint_addr> const& trade_ports,
            std::vector<endpoint_addr> const& quote_ports,
            market_data_processor& processor )
        : trade_thread_size_(trade_thread_size),
        quote_thread_size_(quote_thread_size),
        processor_( processor )
    { 
        for(std::vector<endpoint_addr>::const_iterator it = trade_ports.begin();
                it != trade_ports.end(); ++it)
        {
            conn_ptr c_ptr(new conn(*this, *it, true));
            v_conn_.push_back(c_ptr);
            c_ptr->socket_reload_();
            c_ptr->register_listen_();
        }
        for(std::vector<endpoint_addr>::const_iterator it = quote_ports.begin();
                it != quote_ports.end(); ++it)
        {
            conn_ptr c_ptr(new conn(*this, *it, false));
            v_conn_.push_back(c_ptr);
            c_ptr->socket_reload_();
            c_ptr->register_listen_();
        }
    }

    market_data_receiver::~market_data_receiver()
    {
        try { this->stop(); } catch(...){}
    }

    market_data_receiver::conn::conn(
            market_data_receiver& receiver
            , endpoint_addr addr
            , bool trade_type)
        :
            receiver_(receiver)
            , addr_(addr)
            , trade_type_(trade_type)
    {
        socket_.reset(new boost::asio::ip::udp::socket( receiver_.io_service_ ) );
        endpoint_.reset( new boost::asio::ip::udp::endpoint( boost::asio::ip::address::from_string( "0.0.0.0" ), addr.second ));
    }

    void market_data_receiver::start()
    {
        boost::thread service_thread( boost::bind( &boost::asio::io_service::run, boost::ref( io_service_ )));
        
        for(size_t i = 0; i < trade_thread_size_; ++i)
            t_group_.create_thread(boost::bind(&market_data_receiver::process_trades_, this));

        for(size_t i = 0; i < quote_thread_size_; ++i)
            t_group_.create_thread(boost::bind(&market_data_receiver::process_quotes_, this));

        stop_ = false;
    }

    void market_data_receiver::stop()
    {
        io_service_.stop();
        stop_ = true;
        trade_queue_.stop();
        quote_queue_.stop();
        t_group_.join_all();
    }

    void market_data_receiver::split_block(message_type const& m, std::vector<message_type>& v)
    {
        message_type msg;
        for(message_type::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            char c = *it;
            switch(c)
            {
                case 0x01:
                    msg.clear();
                    break;
                case 0x1f:
                    v.push_back(msg);
                    msg.clear();
                    break;
                case 0x03:
                    v.push_back(msg);
                    return;
                default:
                    msg.push_back(c);
            }
        }
    }

    void market_data_receiver::convert_messages_to_trades(
            std::vector<message_type> const& m
            , std::vector< trade_message_ptr >& trades)
    {
        for(std::vector<message_type>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            message_type m = *it;
            try
            {
                if(!m.empty() &&
                        (parser<long_trade_message>::is_valid_type(m) || parser<short_trade_message>::is_valid_type(m)) )
                {
                    trade_message_ptr t( new trade_message( m ) );
                    trades.push_back( t );
                } else
                {
                    std::cerr << "incorrect trade message" << std::endl;
                }
            } catch(std::logic_error& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    void market_data_receiver::convert_messages_to_quotes(
            std::vector<message_type> const& m
            , std::vector< quote_message_ptr >& quotes)
    {
        for(std::vector<message_type>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            message_type m = *it;
            try
            {
                if(!m.empty() &&
                        ( parser<long_quote_message>::is_valid_type(m) || parser<short_quote_message>::is_valid_type(m)))
                {
                    quote_message_ptr q( new quote_message( m ) );
                    quotes.push_back( q );
                } else
                {
                    std::cout << "incorrect quote message" << std::endl;
                }
            } catch(std::logic_error& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    void market_data_receiver::convert_block_to_trades(
            message_type const& ts_block
            , std::vector< trade_message_ptr >& trades)
    {
        std::vector<message_type> messages;
        split_block(ts_block, messages);
        convert_messages_to_trades(messages, trades);
    }

    void market_data_receiver::convert_block_to_quotes(
            message_type const& ts_block
            , std::vector< quote_message_ptr >& quotes)
    {
        std::vector<message_type> messages;
        split_block(ts_block, messages);
        convert_messages_to_quotes(messages, quotes);
    }

    void market_data_receiver::process_trades_()
    {
        while(true)
        {
            message_type s;
            trade_queue_.wait_and_pop( s );
            if(stop_)
            {
                break;
            }
            std::vector< trade_message_ptr > trades;
            convert_block_to_trades( s, trades );
            for(std::vector<trade_message_ptr>::iterator it = trades.begin(); it != trades.end(); ++it)
            {
                processor_.new_trade( *it );
            }
        }
    }
    void market_data_receiver::process_quotes_()
    {
        while(true)
        {
            message_type s;
            quote_queue_.wait_and_pop( s );
            if(stop_)
            {
                break;
            }
            std::vector< quote_message_ptr > quotes;
            convert_block_to_quotes( s, quotes );
            for(std::vector< quote_message_ptr >::iterator it = quotes.begin(); it != quotes.end(); ++it)
            {
                processor_.new_quote( *it );
            }
        }
    }

    void market_data_receiver::conn::socket_reload_()
    {
        using boost::asio::ip::udp;
        using boost::asio::ip::address;
        using boost::asio::ip::multicast::join_group;

        socket_->open( endpoint_->protocol() );
        socket_->set_option( udp::socket::reuse_address( true ) );
        socket_->bind( *endpoint_ );
        socket_->set_option( join_group( address::from_string( addr_.first ) ) );
    }

    void market_data_receiver::conn::register_listen_( buffer_type pre_buffer, const size_t previous_size )
    {
        buffer_type buffer;

        if( pre_buffer )
            buffer = pre_buffer;
        else
            buffer.reset( new message_type( default_buffer_size, '\0' ) );

        char* const buffer_start = &(*buffer->begin());

        using namespace boost::asio::placeholders;
        socket_->async_receive( boost::asio::buffer( buffer_start, previous_size + default_buffer_size ),
                boost::bind( &market_data_receiver::conn::listen_handler_, this, buffer, error, bytes_transferred ) );
    }

    void market_data_receiver::conn::listen_handler_( buffer_type bt, const boost::system::error_code& error, const size_t bytes_received )
    {
        message_type message(bt->begin(), bt->begin() + bytes_received );
        if(trade_type_)
        {
            boost::mutex::scoped_lock lock( receiver_.protect_trade_queue_ );
            receiver_.trade_queue_.push( message );
        }
        else
        {
            boost::mutex::scoped_lock lock( receiver_.protect_quote_queue_ );
            receiver_.quote_queue_.push( message );
        }
        this->register_listen_( );
    }

}
