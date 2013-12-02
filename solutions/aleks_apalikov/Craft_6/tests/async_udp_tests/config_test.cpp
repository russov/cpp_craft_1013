#include "test_registrator.h"
#include <string>
#include "config.h"

#include <udp_listener.h>

#include <cstring>

void config_test::read_config()
{
	BOOST_CHECK_NO_THROW
	(
		config c("F:\\Cpp_Craft\\cpp\\solutions\\aleks_apalikov\\Craft_6\\sources\\" + string("config.ini"));
	);
}