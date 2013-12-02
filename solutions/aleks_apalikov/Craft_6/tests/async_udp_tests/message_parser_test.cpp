#include "test_registrator.h"
#include <string>
#include "message_parser.h"
#include "config.h"
#include <sstream>

void text_test::quote_trade_parse()
{
	{
//		const trad* t1 = &trade::short_tr;
//		const trad* t2 = &trade::long_tr;
		char v1 = trade::short_tr.vol_of;
		char v2 = trade::long_tr.vol_of;
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
