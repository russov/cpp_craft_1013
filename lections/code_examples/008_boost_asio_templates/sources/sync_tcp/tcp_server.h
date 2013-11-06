#ifndef _SYNC_TCP_TCP_SERVER_H_
#define _SYNC_TCP_TCP_SERVER_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace sync_tcp
{
	class tcp_server : virtual public boost::noncopyable
	{
	};
}

#endif // _SYNC_TCP_TCP_SERVER_H_
