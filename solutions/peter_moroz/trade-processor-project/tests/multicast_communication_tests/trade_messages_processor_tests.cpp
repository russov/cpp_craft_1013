#include <vector>
#include "test_registrator.h"

#include <trade_messages_processor.h>


void multicast_communication::tests_::trade_messages_processor_tests()
{
	BOOST_CHECK_NO_THROW
	( 
		trade_messages_processor tm_processor;
	)

	{
		trade_messages_processor tm_processor;
		std::string msg;
		trade_messages_processor* tm_parser = NULL;
		BOOST_CHECK_NO_THROW( (tm_parser = tm_processor.get_parser(msg)) );
		trade_messages_processor* tm_processor_p = &tm_processor;
		BOOST_CHECK_EQUAL( tm_parser, tm_processor_p );
		trade_message_ptr tmessage_ptr;
		BOOST_CHECK_NO_THROW( (tmessage_ptr = tm_parser->parse_message(msg)) );
		
		BOOST_CHECK_EQUAL( tmessage_ptr->security_symbol(), "" );
		BOOST_CHECK_EQUAL( tmessage_ptr->price(), 0.0 );
		BOOST_CHECK_EQUAL( tmessage_ptr->volume(), 0.0 );
	}
	
	{
		trade_messages_processor tm_processor;

		const size_t messages_count = 16;

		std::vector<trade_message_ptr> msg_p;

		for (size_t i = 0; i < messages_count; ++i)
		{
			std::string msg;
			trade_messages_processor* tm_parser = NULL;
			BOOST_CHECK_NO_THROW( (tm_parser = tm_processor.get_parser(msg)) );
			trade_messages_processor* tm_processor_p = &tm_processor;
			BOOST_CHECK_EQUAL( tm_parser, tm_processor_p );
			trade_message_ptr tmessage_ptr;
			BOOST_CHECK_NO_THROW( (tmessage_ptr = tm_parser->parse_message(msg)) );
			msg_p.push_back(tmessage_ptr);
		}

		BOOST_CHECK_EQUAL( msg_p.size(), messages_count );

		for (size_t i = 0; i < msg_p.size(); ++i)
		{
			BOOST_CHECK_EQUAL( msg_p[i]->security_symbol(), "" );
			BOOST_CHECK_EQUAL( msg_p[i]->price(), 0.0 );
			BOOST_CHECK_EQUAL( msg_p[i]->volume(), 0.0 );
		}
		
	}
}
