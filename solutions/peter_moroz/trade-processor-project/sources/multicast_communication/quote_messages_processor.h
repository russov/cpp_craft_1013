#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGES_PROCESSOR_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGES_PROCESSOR_H_

#include "quote_message.h"

namespace multicast_communication
{

  // I've used 'Chain-Of-Responsibility' design pattern here 
  // To decide which kind of CQS-message we need to process
  class quote_messages_processor
  {
  public:
    explicit quote_messages_processor();
    virtual ~quote_messages_processor();

  private:
    void add_element_to_chain(quote_messages_processor* next);

  public:
    void initialize();

  protected:
    virtual bool is_parseable(const std::string& msg) { return true; }

  public:
    // TO DO: Replace raw-pointer by suitable kind of smart-pointer.
    quote_messages_processor* get_parser(const std::string msg);
    virtual quote_message_ptr parse_message(const std::string& msg)
    {
      return quote_message_ptr(new quote_message());
    }

  private:
    quote_messages_processor* next_;
  };

  typedef boost::shared_ptr<quote_messages_processor> quote_messages_processor_ptr;


  class long_quote_messages_processor : public quote_messages_processor
  {
  public:
    explicit long_quote_messages_processor() {}
    virtual ~long_quote_messages_processor() {}

  protected:
    virtual bool is_parseable(const std::string& msg);

  public:
    virtual quote_message_ptr parse_message(const std::string& msg);
  };


  class short_quote_messages_processor : public quote_messages_processor
  {
  public:
    explicit short_quote_messages_processor() {}
    virtual ~short_quote_messages_processor() {}

  protected:
    virtual bool is_parseable(const std::string& msg);

  public:
    virtual quote_message_ptr parse_message(const std::string& msg);
  };
}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGES_PROCESSOR_H_
