#include "market_data_processor.h"

int market_data_processor::wr_trade( shared_trade trad )
{
	outp << "T "<< trad->security_symbol() <<std::setprecision(2) << ( trad->price() / trad->denom()) 
		<< std::setprecision(1) << trad->volume();
	return 1;
}

int market_data_processor::wr_quote( shared_quote quot )
{
	outp << "Q " << std::fixed << quot->security_symbol() <<std::setprecision(2) << ( quot->bid_price() / quot->bid_denom() )
		<< std::setprecision(1) << quot->bid_volume()  <<std::setprecision(2) << ( quot->offer_price() / quot->offer_denom() )
		<< std::setprecision(1) << quot->offer_volume();
	return 1;
}

