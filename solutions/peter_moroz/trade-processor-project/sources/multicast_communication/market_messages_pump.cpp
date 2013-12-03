#include "market_messages_pump.h"

namespace mcast_comm = multicast_communication;

mcast_comm::market_messages_pump::market_messages_pump(
    boost::asio::io_service& io_service,
    const message_receiver_delegate_ptr& msg_recv_delegate)
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
  std::string msg(buffer, recv_bytes_count);
  msg_recv_delegate_->on_message_received(msg);
  receive();
}
