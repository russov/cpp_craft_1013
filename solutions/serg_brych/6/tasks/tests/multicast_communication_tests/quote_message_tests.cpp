#include "test_registrator.h"

#include <quote_message.h>

void multicast_communication::tests_::quote_message_tests()
{
	{
		BOOST_CHECK_NO_THROW
		(
			quote_message qm;

			BOOST_CHECK_EQUAL( qm.security_symbol(), "" );
			BOOST_CHECK_EQUAL( qm.bid_price(), 0.0 );
			BOOST_CHECK_EQUAL( qm.bid_volume(), 0.0 );
			BOOST_CHECK_EQUAL( qm.offer_price(), 0.0 );
			BOOST_CHECK_EQUAL( qm.offer_volume(), 0.0 );
		)
	}

	// test short quote
	{
		std::stringstream ss("EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12");
		quote_message qm;
		BOOST_CHECK_NO_THROW
		(
			qm.parse_quote(ss);
		)
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ADM" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 41.47 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 6.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 41.48 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 4.0 );
	}

	// test long quote
	{
		std::stringstream ss("EBEO A  003759035N:J_077ALU             0    AAAR CD0000000423000000147D0000000424000000380     A   02");
		quote_message qm;

		BOOST_CHECK_NO_THROW(
			qm.parse_quote(ss);
		)	
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ALU        " );
		BOOST_CHECK_EQUAL( qm.bid_price(), 4.23 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 147.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 4.24 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 380.0 );
		
	}
}
