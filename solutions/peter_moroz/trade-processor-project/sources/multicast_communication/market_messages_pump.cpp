#include "market_messages_pump.h"

namespace mcast_comm = multicast_communication;

mcast_comm::market_messages_pump::market_messages_pump(
    boost::asio::io_service& io_service,
    const market_data_received_delegate_ptr& msg_recv_delegate)
  : started_(false), socket_(io_service), 
    msg_recv_delegate_(msg_recv_delegate)
{
}
mcast_comm::market_messages_pump::~market_messages_pump() {}

void mcast_comm::market_messages_pump::start(const std::string& addr,
                                             unsigned short port)
{
  if (started())
    return;
  started_ = true;

  using boost::asio::ip::udp;
  using boost::asio::ip::address;
  using boost::asio::ip::multicast::join_group;
  
  boost::asio::ip::udp::endpoint ep(address::from_string("0.0.0.0"), port);
  socket_.open(ep.protocol());
  socket_.set_option(udp::socket::reuse_address(true));
  socket_.bind(ep);
  socket_.set_option(join_group(address::from_string(addr)));

  receive();
}
void mcast_comm::market_messages_pump::stop()
{
  if (!started())
    return;
  started_ = false;
  socket_.close();
}

void mcast_comm::market_messages_pump::receive()
{
  using namespace boost::asio::placeholders;

  socket_.async_receive(boost::asio::buffer(recv_buffer_),
            boost::bind(&market_messages_pump::on_receive, 
            this, recv_buffer_, error, bytes_transferred));
}
void mcast_comm::market_messages_pump::on_receive(
    char* buffer, const sys_err_code& error, const size_t recv_bytes_count)
{
  if (error)
    return;
  transmission_block_.assign(recv_buffer_, recv_bytes_count);
  parse_transmission_block();
  receive();
}

void mcast_comm::market_messages_pump::parse_transmission_block()
{
  size_t p0 = transmission_block_.find(SOH);
  size_t p1 = 0;
  
  // we have uncompleted message, which has
  // appeared during previous receive session
  if (message_.size() != 0)
  {
    // search end of previous block
    size_t p = transmission_block_.find(ETX);
    if (p == std::string::npos)
    {
      message_.clear();
      return;
    }
    std::string tail_of_prev_msg = transmission_block_.substr(0, p);
    message_.append(tail_of_prev_msg);
    transfer_message();
  } // if (message_.size() != 0)

  // usual processing of transmission block
  while (p0 < transmission_block_.size() && p0 != std::string::npos)
  {
    p1 = transmission_block_.find(US, p0);
    if (p1 == std::string::npos)      // last message in block
    {
      p1 = transmission_block_.find(ETX, p0);
      if (p1 == std::string::npos)    // probably uncompleted message
      {
        message_ = transmission_block_.substr(p0);
        break;
      }
    }
    message_ = transmission_block_.substr(p0, p1 - p0);
    transfer_message();
    p0 = p1 + 1;
  } // while (p0 < transmission_block_.size() && p0 != std::string::npos)
}
void mcast_comm::market_messages_pump::transfer_message()
{
  msg_recv_delegate_->on_message_received(message_);
  message_.clear();
}
