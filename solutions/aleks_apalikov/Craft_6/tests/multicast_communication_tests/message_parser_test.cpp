#include "test_registrator.h"
#include <string>
#include "message_parser.h"
#include "config.h"
#include <sstream>

void text_test::quote_trade_parse()
{
	{
		BOOST_CHECK_THROW(
			message::denominator('J'),
			std::logic_error
		);
		BOOST_CHECK_EQUAL(message::denominator('8'), 256.0);
		BOOST_CHECK_EQUAL(message::denominator('C'), 1000.0);
		BOOST_CHECK_EQUAL(message::denominator('H'), 100000000.0);
		BOOST_CHECK_EQUAL(message::denominator('I'), 1.0);
	}
	{
		stringstream ss;
		string str = data_path + string("1.udp");
		ofstream ofs; 
		ofs.open(str.c_str());
		ss << "EBEO A  003759557N:J_735AVB             0    AAAR B30000012127000000001D0000012137000000001     A   62TB00012130001 BB00012137001 "
			<<"EDEO A  003759121P:J_428AINR  D00352000001 F00354300001 02"
			<<"LDEO A  003759122N:J_432ALJR  F00124900003 D00125100001 02";
		copy(
			istreambuf_iterator<char>(ss),
			istreambuf_iterator<char>(),
			ostreambuf_iterator<char>(ofs));
		quote q(ss);
		q.read_next();
		BOOST_CHECK_EQUAL(q.bid_denom, '3');
		BOOST_CHECK_EQUAL(q.bid_price, 1212700.0);
		BOOST_CHECK_EQUAL(q.bid_volume, 1);
		BOOST_CHECK_EQUAL(q.offer_denom, 'D');
		BOOST_CHECK_EQUAL(q.offer_price, 1213700.0);
		BOOST_CHECK_EQUAL(q.offer_volume, 1);
		BOOST_CHECK_EQUAL((char)q.get_categ(), 'E');
		q.read_next();
		BOOST_CHECK_EQUAL(q.bid_denom, 'D');
		BOOST_CHECK_EQUAL(q.bid_price, 352000.0);
		BOOST_CHECK_EQUAL(q.bid_volume, 1);
		BOOST_CHECK_EQUAL(q.offer_denom, 'F');
		BOOST_CHECK_EQUAL(q.offer_price, 354300.0);
		BOOST_CHECK_EQUAL(q.offer_volume, 1);
		BOOST_CHECK_EQUAL((char)q.get_type(), 'D');
			q.read_next();
		BOOST_CHECK_EQUAL((char)q.bid_denom, 'F');
		BOOST_CHECK_EQUAL((char)q.get_type(), 'D');
		BOOST_CHECK_EQUAL((char)q.get_categ(), 'L');
		BOOST_CHECK_NO_THROW(
			q.read_next();
		);
		BOOST_CHECK_EQUAL((char)q.get_categ(), (char)-1);
		ofs.close();

	}
	{
		const quote::quote_t& quot = quote::get_long();
		BOOST_CHECK_EQUAL(quot.off_denom_of, 36);
		BOOST_CHECK_EQUAL(quot.bid_vol_len, quot.off_vol_len);
		BOOST_CHECK_EQUAL(quot.bid_vol_len, 7);

		string str = data_path + string("233.200.79.0.udp");
		ifstream inp; 
		inp.open (str.c_str());
		quote q(inp);
		q.read_next();
		while( q.get_categ() != -1)
		{
			q.read_next();
		}
	}
	
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
