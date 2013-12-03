#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGES_PROCESSOR_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGES_PROCESSOR_H_

#include "trade_message.h"

namespace multicast_communication
{

  // I've used 'Chain-Of-Responsibility' design pattern here 
  // To decide which kind of CQS-message we need to process
  class trade_messages_processor
  {
  public:
    explicit trade_messages_processor();
    virtual ~trade_messages_processor();

  private:
    void add_element_to_chain(trade_messages_processor* next);

  public:
    void initialize();

  protected:
    virtual bool is_parseable(const std::string& msg) { return true; }

  public:
    trade_messages_processor* get_parser(const std::string msg);
    virtual trade_message_ptr parse_message(const std::string& msg)
    {
      return trade_message_ptr(new trade_message());
    }

  private:
    trade_messages_processor* next_;   
  };

  typedef boost::shared_ptr<trade_messages_processor> trade_messages_processor_ptr;


  class long_trade_messages_processor : public trade_messages_processor
  {
  public:
    explicit long_trade_messages_processor() {}
    virtual ~long_trade_messages_processor() {}

  protected:
    virtual bool is_parseable(const std::string& msg);

  public:
    virtual trade_message_ptr parse_message(const std::string& msg);
  };


  class short_trade_messages_processor : public trade_messages_processor
  {
  public:
    explicit short_trade_messages_processor() {}
    virtual ~short_trade_messages_processor() {}

  protected:
    virtual bool is_parseable(const std::string& msg);

  public:
    virtual trade_message_ptr parse_message(const std::string& msg);
  };
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGES_PROCESSOR_H_
