#ifndef _MULTICAST_COMMUNICATION_MARKET_MESSAGES_PUMP_H_
#define _MULTICAST_COMMUNICATION_MARKET_MESSAGES_PUMP_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../market_data_receiver/market_data_receiver.h"

namespace multicast_communication
{
  typedef std::pair<std::string, unsigned int> connection_point;

  inline boost::asio::ip::udp::endpoint cp_to_ep(const connection_point cp)
  {
    using namespace boost::asio;
    return ip::udp::endpoint(ip::address::from_string(cp.first), cp.second);
  }

  // class responsible for receiving data from network, parsing 
  // stream of bytes into messages and buffering uncompleted messages
  class market_messages_pump
  {
    static const size_t kRecvBufferSize = 1000;
    typedef boost::system::error_code sys_err_code;

  public:
    explicit market_messages_pump(boost::asio::io_service& io_service,
              const message_receiver_delegate_ptr& msg_recv_delegate);
    virtual ~market_messages_pump();

  public:
    void start(const std::string& addr, unsigned short port);
    void stop();
    bool started() const { return started_; }

  private:
    void receive();
    void on_receive(char* buffer, const sys_err_code& error, 
                    const size_t recv_bytes_count);

  private:
    bool started_;
    boost::asio::ip::udp::socket socket_;
    message_receiver_delegate_ptr msg_recv_delegate_;
    char recv_buffer_[kRecvBufferSize];
  };

  typedef boost::shared_ptr<market_messages_pump> market_messages_pump_ptr;
}

#endif // _MULTICAST_COMMUNICATION_MARKET_MESSAGES_PUMP_H_
