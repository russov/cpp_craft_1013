#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include <vector>
#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "parser.h"
#include "util.h"

namespace multicast_communication
{
    class quote_message: virtual private boost::noncopyable
    { 
        public:
            quote_message(message_type const&);
            std::string security_symbol() const;
            double bid_price() const;
            double bid_volume() const;
            double offer_price() const;
            double offer_volume() const;
            static bool is_valid_type(message_type const&);
            friend std::ostream& operator<<(std::ostream&, quote_message const&);
        private:

            template <typename T>
                void parse_raw()
                {
                    parser<T> p(raw_);
                    category_ = raw_[0];
                    type_ = raw_[1];
                    security_symbol_ = p.template extract<std::string>(T::SECURITY_SYMBOL);                    
                    bid_price_demoninator_indicator_ = p.template extract<char>(T::BID_PRICE_DENOMINATOR_INDICATOR);
                    bid_price_ = p.template extract<double>(T::BID_PRICE) 
                        / util::get_denom(bid_price_demoninator_indicator_);
                    bid_size_ = p.template extract<double>(T::BID_SIZE);
                    offer_price_denominator_indicator_ = p.template extract<char>(T::OFFER_PRICE_DENOMINATOR_INDICATOR);
                    offer_price_ = p.template extract<double>(T::OFFER_PRICE) 
                        / util::get_denom(offer_price_denominator_indicator_);
                    offer_size_ = p.template extract<double>(T::OFFER_SIZE);
                }

            message_type raw_;
            std::string security_symbol_;
            char bid_price_demoninator_indicator_;
            double bid_price_;
            double bid_size_;
            char offer_price_denominator_indicator_;
            double offer_price_;
            double offer_size_;
            char category_;
            char type_;
    };

    struct long_quote_message
    {
        static const int FIELDS = 30;
        static const int SIZE = 102;

        static const size_t RESTRICTION_SIZE = 3;
        enum
        {
            MESSAGE_HEADER,
            SECURITY_SYMBOL,
            TEMPORARY_SUFFIX,
            TEST_MESSAGE_INDICATOR,
            PRIMARY_LISTING_MARKET_PARTICIPANT_IDENTIFIER,
            SIP_GENERATED_MESSAGE_IDENTIFIER,
            RESERVED,
            FINANCIAL_STATUS,
            CURRENCY_INDICATOR,
            INSTRUMENT_TYPE,
            CANCEL_CORRECTION_INDICATOR,
            SETTLEMENT_CONDITION,
            MARKET_CONDITION,
            QUOTE_CONDITION,
            LIMIT_UP_LIMIT_DOWN_LULD_INDICATOR,
            RETAIL_INTEREST_INDICATOR,
            BID_PRICE_DENOMINATOR_INDICATOR,
            BID_PRICE,
            BID_SIZE,
            OFFER_PRICE_DENOMINATOR_INDICATOR,
            OFFER_PRICE,
            OFFER_SIZE,
            FINRA_MARKET_MAKER_ID,
            RESERVED_2,
            NATIONAL_BBO_LULD_INDICATOR,
            FINRA_BBO_LULD_INDICATOR,
            SHORT_SALE_RESTRICTION_INDICATOR,
            RESERVED_3,
            NATIONAL_BBO_INDICATOR,
            FINRA_BBO_INDICATOR
        };
        static const int LENGTH[FIELDS];
        static const int OFFSET[FIELDS];

        static const char RESTRICTION_TYPE[RESTRICTION_SIZE];
        static const char RESTRICTION_CATEGORY[RESTRICTION_SIZE];
    };

    struct short_quote_message
    {
        static const int FIELDS = 15;
        static const int SIZE = 58;

        static const size_t RESTRICTION_SIZE = 3;
        enum
        {
            MESSAGE_HEADER,
            SECURITY_SYMBOL,
            QUOTE_CONDITION,
            LIMIT_UP_LIMIT_DOWN_LULD_INDICATOR,
            RESERVED_1,
            BID_PRICE_DENOMINATOR_INDICATOR,
            BID_PRICE,
            BID_SIZE,
            RESERVED_2,
            OFFER_PRICE_DENOMINATOR_INDICATOR,
            OFFER_PRICE,
            OFFER_SIZE,
            RESERVED_3,
            NATIONAL_BBO_INDICATOR,
            FINRA_BBO_INDICATOR,
            TOTAL_LENGTH
        };
        static const int LENGTH[FIELDS];
        static const int OFFSET[FIELDS];

        static const char RESTRICTION_TYPE[RESTRICTION_SIZE];
        static const char RESTRICTION_CATEGORY[RESTRICTION_SIZE];
    };

    typedef boost::shared_ptr< quote_message > quote_message_ptr;
}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
