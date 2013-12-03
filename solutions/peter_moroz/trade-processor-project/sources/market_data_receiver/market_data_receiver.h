#ifndef _MARKET_DATA_RECEIVER_MARKET_DATA_RECEIVER_H_
#define _MARKET_DATA_RECEIVER_MARKET_DATA_RECEIVER_H_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "../multicast_communication/market_data_processor.h"
#include "../multicast_communication/quote_messages_processor.h"
#include "../multicast_communication/trade_messages_processor.h"

namespace multicast_communication
{
  // I have to make all implementation of given class inside header-file,
  // because, if I did it inside usual cpp-file, I have got error
  // messages, when linker try to resolve names, which aren't belong
  // to multicast_communication-library. So, main application was built 
  // succesfully, but building of test environment wasn't so nice.
  class market_data_receiver
  {
  public:
    market_data_receiver(market_data_processor_ptr& market_data_processor_p)
      : market_data_processor_(market_data_processor_p),
        quote_messages_processor_(new quote_messages_processor()),
        trade_messages_processor_(new trade_messages_processor())
    {
    }

  public:
    void on_quote_message(const std::string& msg)
    {
      quote_messages_processor* qmessage_parser = NULL;
      qmessage_parser = quote_messages_processor_->get_parser(msg);
      if (qmessage_parser != NULL)
      {
        quote_message_ptr qmessage = qmessage_parser->parse_message(msg);
        if (qmessage != NULL)
          market_data_processor_->new_quote(qmessage);
      } 
    }
    void on_trade_message(const std::string& msg)
    {
      trade_messages_processor* tmessage_parser = NULL;
      tmessage_parser = trade_messages_processor_->get_parser(msg);
      if (tmessage_parser != NULL)
      {
        trade_message_ptr tmessage = tmessage_parser->parse_message(msg);
        if (tmessage != NULL)
          market_data_processor_->new_trade(tmessage);
      } 
    }
  
  private:
    market_data_processor_ptr market_data_processor_;
    boost::scoped_ptr<quote_messages_processor> quote_messages_processor_;
    boost::scoped_ptr<trade_messages_processor> trade_messages_processor_;
  };

  typedef boost::shared_ptr<market_data_receiver> market_data_receiver_ptr;


  class message_receiver_delegate
  {
  protected:
    message_receiver_delegate(const market_data_receiver_ptr& receiver) 
      : receiver_(receiver) {}

  public:
    virtual void on_message_received(const std::string& msg) = 0;
  protected:
    market_data_receiver_ptr receiver_;
  };

  typedef boost::shared_ptr<message_receiver_delegate> message_receiver_delegate_ptr;


  class qmessage_receiver_delegate : public message_receiver_delegate
  {
  public:
    qmessage_receiver_delegate(const market_data_receiver_ptr& receiver) 
      : message_receiver_delegate(receiver) {}
      
  public:
    virtual void on_message_received(const std::string& msg)
    {
      receiver_->on_quote_message(msg);
    }
  };

  class tmessage_receiver_delegate : public message_receiver_delegate
  {
  public:
    tmessage_receiver_delegate(const market_data_receiver_ptr& receiver) 
      : message_receiver_delegate(receiver) {}
      
  public:
    virtual void on_message_received(const std::string& msg)
    {
      receiver_->on_trade_message(msg);
    }
  };

}

#endif // _MARKET_DATA_RECEIVER_DATA_RECEIVER_H_