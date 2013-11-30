#include <iostream>
#include <fstream>
#include <boost/thread.hpp>

#include "market_data_processor.h"
#include "trade_message.h"
#include "quote_message.h"

namespace multicast_communication
{
    void market_data_processor_impl::new_trade(trade_message_ptr const message)
    {
        boost::mutex::scoped_lock lock(protect_trades_);
        trades_.push_back( message );
    }

    void market_data_processor_impl::new_quote(quote_message_ptr const message)
    {
        boost::mutex::scoped_lock lock(protect_quotes_);
        quotes_.push_back( message );
    }

    std::vector< trade_message_ptr > const& market_data_processor_impl::trades() const
    { 
        boost::mutex::scoped_lock lock(protect_trades_);
        return trades_;
    }
    std::vector< quote_message_ptr > const& market_data_processor_impl::quotes() const
    { 
        boost::mutex::scoped_lock lock(protect_quotes_);
        return quotes_;
    }

    void market_data_processor_impl::dump(std::string const& filename) const
    { 
        std::ofstream os(filename, std::ios_base::out);
        for(std::vector< trade_message_ptr >::const_iterator it = trades_.begin(); it != trades_.end(); ++it)
        {
            trade_message_ptr m = *it;
            os << "T " << m->security_symbol() << " " << m->price() << " " << m->volume() << std::endl;
        }
        for(std::vector< quote_message_ptr >::const_iterator it = quotes_.begin(); it != quotes_.end(); ++it)
        {
            quote_message_ptr m = *it;
            os << "Q " << m->security_symbol() << " " << m->bid_price() << " " << m->bid_volume() << " " << m->offer_price() << " " << m->offer_volume() << std::endl;
        }
    } 
}
