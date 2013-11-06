#ifndef _SYNC_TCP_TCP_CLIENT_H_
#define _SYNC_TCP_TCP_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace sync_tcp
{
	class tcp_client : virtual public boost::noncopyable
	{
	};
}

#endif // _SYNC_TCP_TCP_CLIENT_H_
