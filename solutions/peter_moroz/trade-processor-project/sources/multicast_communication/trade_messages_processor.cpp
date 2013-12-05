#include <stdexcept>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "trade_messages_processor.h"

namespace mcast_comm = multicast_communication;

mcast_comm::trade_messages_processor::trade_messages_processor() 
  : next_(NULL)
{
}
mcast_comm::trade_messages_processor::~trade_messages_processor()
{
  delete next_;
}

void mcast_comm::trade_messages_processor::add_element_to_chain(
                      mcast_comm::trade_messages_processor* next)
{
  std::stringstream err_message;
  if (next == NULL)
  {
    err_message 
      << "Method: trade_messages_processor::add_element_to_chain "
      << " Error: NULL-pointer has been passed as argument.\n";
    throw std::invalid_argument(err_message.str());
  }

  if (next->next_ != NULL)
  {
    err_message 
      << "Method: trade_messages_processor::add_element_to_chain "
      << " Error: Not terminated chain element has been passed.\n";
    throw std::invalid_argument(err_message.str());
  }

  if (next_ == NULL)
    next_ = next;
  else
    add_element_to_chain(next);
}

// everyone, who want to add some extra handlers to chain
// need to change method initialize()
void mcast_comm::trade_messages_processor::initialize()
{
  add_element_to_chain(new long_trade_messages_processor());
  add_element_to_chain(new short_trade_messages_processor());
}

mcast_comm::trade_messages_processor* 
    mcast_comm::trade_messages_processor::get_parser(const std::string msg)
{
  if (is_parseable(msg))
    return this;

  if (next_ != NULL)
    return next_->get_parser(msg);

  return NULL;
}

bool mcast_comm::long_trade_messages_processor::is_parseable(const std::string& m)
{
  return false;
}

mcast_comm::trade_message_ptr
    mcast_comm::long_trade_messages_processor::parse_message(const std::string& m)
{
  std::string security_symbol = "";
  std::string price = "";
  std::string volume = "";
	
  double p = 0.0;
  double v = 0.0;
  
  try {
    p = boost::lexical_cast<double>(price.c_str());
    v = boost::lexical_cast<double>(volume.c_str());
  } catch (const boost::bad_lexical_cast&) {
    return trade_message_ptr();
  }

  trade_message_ptr tmessage_ptr(new trade_message(security_symbol, p, v));
  return tmessage_ptr;
}


bool mcast_comm::short_trade_messages_processor::is_parseable(const std::string& m)
{
  return false;
}
mcast_comm::trade_message_ptr 
    mcast_comm::short_trade_messages_processor::parse_message(const std::string& m)
{
  std::string security_symbol = "";
  std::string price = "";
  std::string volume = "";

  double p = 0.0;
  double v = 0.0;

  try {
    p = boost::lexical_cast<double>(price.c_str());
    v = boost::lexical_cast<double>(volume.c_str());
  } catch (const boost::bad_lexical_cast&) {
    return trade_message_ptr();
  }

  trade_message_ptr tmessage_ptr(new trade_message(security_symbol, p, v));
  return tmessage_ptr;
}
