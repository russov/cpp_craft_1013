#include "test_registrator.h"

#include <trade_message.h>

void multicast_communication::tests_::trade_message_tests()
{
	BOOST_CHECK_NO_THROW
	(
		trade_message tm;

		BOOST_CHECK_EQUAL( tm.security_symbol(), "" );
		BOOST_CHECK_EQUAL( tm.price(), 0.0 );
		BOOST_CHECK_EQUAL( tm.volume(), 0.0 );
	)

	// test short trade
	{
		std::stringstream ss("EIAO A  000140208P:3\\806BGS@0100D00330400DF ");
		trade_message tm;
		BOOST_CHECK_NO_THROW
		(
			tm.parse_trade(ss);
		)
		BOOST_CHECK_EQUAL( tm.security_symbol(), "BGS" );
		BOOST_CHECK_EQUAL( tm.price(),  33.04);
		BOOST_CHECK_EQUAL( tm.volume(), 100.0);
	}


	// test long trade
	{
		std::stringstream ss("EBAO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0");
		trade_message tm;
		BOOST_CHECK_NO_THROW
		(
			tm.parse_trade(ss);
		)
		BOOST_CHECK_EQUAL( tm.security_symbol(), "ACN        " );
		BOOST_CHECK_EQUAL( tm.price(), 77.9 );
		BOOST_CHECK_EQUAL( tm.volume(), 100.0 );
	}
	
		
}
