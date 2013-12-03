#include <stdexcept>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "quote_messages_processor.h"

namespace mcast_comm = multicast_communication;

mcast_comm::quote_messages_processor::quote_messages_processor() 
  : next_(NULL)
{
}
mcast_comm::quote_messages_processor::~quote_messages_processor()
{
  delete next_;
}


void mcast_comm::quote_messages_processor::add_element_to_chain(
    mcast_comm::quote_messages_processor* next)
{
}

// you need to change method initialize(), if 
// you want to add some extra handlers to chain
void mcast_comm::quote_messages_processor::initialize()
{
  add_element_to_chain(new long_quote_messages_processor());
  add_element_to_chain(new short_quote_messages_processor());
}

mcast_comm::quote_messages_processor*
    mcast_comm::quote_messages_processor::get_parser(const std::string msg)
{
  if (is_parseable(msg))
    return this;

  if (next_ != NULL)
    return next_->get_parser(msg);

  return NULL;
}

bool mcast_comm::long_quote_messages_processor::is_parseable(const std::string& m)
{
  return true;
}

mcast_comm::quote_message_ptr 
    mcast_comm::long_quote_messages_processor::parse_message(const std::string& m)
{
  std::string security_symbol = "";
  std::string b_price = "0.0";
  std::string b_volume = "0.0";
  std::string o_price = "0.0";
  std::string o_volume = "0.0";
	
  double bid_price = 0.0;
  double bid_volume = 0.0;
  double offer_price = 0.0;
  double offer_volume = 0.0;

  try {
    bid_price = boost::lexical_cast<double>(b_price.c_str());
    bid_volume = boost::lexical_cast<double>(b_volume.c_str());
    offer_price = boost::lexical_cast<double>(o_price.c_str());
    offer_volume = boost::lexical_cast<double>(o_volume.c_str());
  } catch (const boost::bad_lexical_cast&) {
    return quote_message_ptr();
  }

  quote_message_ptr qmessage_ptr(new quote_message(security_symbol,
                bid_price, bid_volume, offer_price, offer_volume));
  return qmessage_ptr;
}


bool mcast_comm::short_quote_messages_processor::is_parseable(
                                         const std::string& m)
{
  return false;
}
mcast_comm::quote_message_ptr 
    mcast_comm::short_quote_messages_processor::parse_message(const std::string& m)
{
  std::string security_symbol = "";
  std::string b_price = "";
  std::string b_volume = "";
  std::string o_price = "";
  std::string o_volume = "";
	
  double bid_price = 0.0;
  double bid_volume = 0.0;
  double offer_price = 0.0;
  double offer_volume = 0.0;

  try {
    bid_price = boost::lexical_cast<double>(b_price.c_str());
    bid_volume = boost::lexical_cast<double>(b_volume.c_str());
    offer_price = boost::lexical_cast<double>(o_price.c_str());
    offer_volume = boost::lexical_cast<double>(o_volume.c_str());
  } catch (const boost::bad_lexical_cast&) {
    return quote_message_ptr();
  }

  quote_message_ptr qmessage_ptr(new quote_message(security_symbol,
                bid_price, bid_volume, offer_price, offer_volume));
  return qmessage_ptr;
}
