#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "trade_message.h"
#include "quote_message.h"

namespace multicast_communication
{
    class market_data_processor : virtual protected boost::noncopyable
    {
        friend class market_data_receiver;
        public:
        explicit market_data_processor(){}
        virtual ~market_data_processor(){}
        virtual std::vector< trade_message_ptr > const& trades() const = 0;
        virtual std::vector< quote_message_ptr > const& quotes() const = 0;
        virtual void dump(std::string const&) const = 0;
        private:
        virtual void new_trade( const trade_message_ptr ) = 0;
        virtual void new_quote( const quote_message_ptr ) = 0;
    };

    class market_data_processor_impl : public market_data_processor
    {
        public:
            explicit market_data_processor_impl() { }
            ~market_data_processor_impl(){}
            std::vector< trade_message_ptr > const& trades() const;
            std::vector< quote_message_ptr > const& quotes() const;
            void dump(std::string const&) const;
        private:
            void new_trade( trade_message_ptr const );
            void new_quote( quote_message_ptr const ); 

            mutable boost::mutex protect_trades_;
            mutable boost::mutex protect_quotes_;

            std::vector< trade_message_ptr > trades_;
            std::vector< quote_message_ptr > quotes_;
    };

}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_
