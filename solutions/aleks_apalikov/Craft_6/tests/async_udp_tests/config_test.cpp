#include "test_registrator.h"
#include <string>
#include "config.h"

#include <udp_listener.h>

#include <cstring>

void config_test::read_config()
{
	BOOST_CHECK_NO_THROW
	(
		config c(data_path + string("config.ini"));
	);
	{
		config c(data_path + string("config.ini"));
		BOOST_CHECK_EQUAL (c.get_trades().size(), c.trade_ports());
		BOOST_CHECK_EQUAL (c.get_quotes().size(), c.quote_ports());
	}
}