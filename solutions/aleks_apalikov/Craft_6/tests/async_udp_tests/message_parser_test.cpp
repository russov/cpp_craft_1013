#include "test_registrator.h"
#include <string>
#include "message_parser.h"
#include "config.h"
#include <sstream>

void text_test::quote_trade_parse()
{
	{
//		trade t12(stringstream());
		const trade::trade_t* t1 = & trade::get_short() ;
		const trade::trade_t& t2 = trade::get_long();
		BOOST_CHECK_EQUAL(t1->denom_of, 5) ;
		BOOST_CHECK_EQUAL(t1->pr_of, 6) ;
		BOOST_CHECK_EQUAL(t1->vol_len, 4);
		BOOST_CHECK_EQUAL(t2.vol_of, 34) ;
		BOOST_CHECK_EQUAL(t2.pr_of, 22) ;
		BOOST_CHECK_EQUAL(t2.denom_of, 21) ;
		string str = data_path + string("233.200.79.0.udp");
		ifstream fs; 
		stringstream sout;
		fs.open (str.c_str());
		copy(istreambuf_iterator<char>(fs),
			istreambuf_iterator<char>(),
			ostreambuf_iterator<char>(sout));
		message m(sout);
		while(m.read_category() != -1)
		{
		}
	}

	{
		string str = data_path + string("233.200.79.128.udp");
		ifstream fs; 
		fs.open (str.c_str());
		trade t(fs);
		t.read_next();
		while( t.get_categ() != -1)
		{
			t.read_next();
		}
	}

	{
	
	}
}
