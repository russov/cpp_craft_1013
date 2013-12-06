#include <vector>
#include "test_registrator.h"

#include <quote_messages_processor.h>


void multicast_communication::tests_::quote_messages_processor_tests()
{
	BOOST_CHECK_NO_THROW
	( 
		quote_messages_processor qm_processor;
	)

	{
		quote_messages_processor qm_processor;
		std::string msg;
		quote_messages_processor* qm_parser = NULL;
		BOOST_CHECK_NO_THROW( (qm_parser = qm_processor.get_parser(msg)) );
		quote_messages_processor* qm_processor_p = &qm_processor;
		BOOST_CHECK_EQUAL( qm_parser, qm_processor_p );
		quote_message_ptr qmessage_ptr;
		BOOST_CHECK_NO_THROW( (qmessage_ptr = qm_parser->parse_message(msg)) );
		
		BOOST_CHECK_EQUAL( qmessage_ptr->security_symbol(), "" );
		BOOST_CHECK_EQUAL( qmessage_ptr->bid_price(), 0.0 );
		BOOST_CHECK_EQUAL( qmessage_ptr->offer_price(), 0.0 );
		BOOST_CHECK_EQUAL( qmessage_ptr->bid_volume(), 0.0 );
		BOOST_CHECK_EQUAL( qmessage_ptr->offer_volume(), 0.0 );
	}
	
	{
		quote_messages_processor qm_processor;

		const size_t messages_count = 16;

		std::vector<quote_message_ptr> msg_p;

		for (size_t i = 0; i < messages_count; ++i)
		{
			std::string msg;
			quote_messages_processor* qm_parser = NULL;
			BOOST_CHECK_NO_THROW( (qm_parser = qm_processor.get_parser(msg)) );
			quote_messages_processor* qm_processor_p = &qm_processor;
			BOOST_CHECK_EQUAL( qm_parser, qm_processor_p );
			quote_message_ptr qmessage_ptr;
			BOOST_CHECK_NO_THROW( (qmessage_ptr = qm_parser->parse_message(msg)) );
			msg_p.push_back(qmessage_ptr);
		}

		BOOST_CHECK_EQUAL( msg_p.size(), messages_count );

		for (size_t i = 0; i < msg_p.size(); ++i)
		{
			BOOST_CHECK_EQUAL( msg_p[i]->security_symbol(), "" );
			BOOST_CHECK_EQUAL( msg_p[i]->bid_price(), 0.0 );
			BOOST_CHECK_EQUAL( msg_p[i]->offer_price(), 0.0 );
			BOOST_CHECK_EQUAL( msg_p[i]->bid_volume(), 0.0 );
			BOOST_CHECK_EQUAL( msg_p[i]->offer_volume(), 0.0 );
		}
		
	}
}
