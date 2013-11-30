#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include <vector>
#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "parser.h"
#include "util.h"

namespace multicast_communication
{
    class trade_message: virtual private boost::noncopyable
    {
        public:
            trade_message(message_type const&);
            std::string security_symbol() const;
            double price() const;
            double volume() const;
            friend std::ostream& operator<<(std::ostream&, trade_message const&);
        private:
            template<typename T>
                void parse_raw()
                {
                    parser<T> p(raw_);
                    category_ = raw_[0];
                    type_ = raw_[1];
                    security_symbol_ = p.template extract<std::string>(T::SECURITY_SYMBOL);
                    price_denom_code_ = p.template extract<char>(T::PRICE_DENOMINATOR_INDICATOR);
                    price_ = p.template extract<double>(T::TRADE_PRICE) / util::get_denom(price_denom_code_);
                    volume_ = p.template extract<double>(T::TRADE_VOLUME);
                }
            message_type raw_;
            std::string security_symbol_;
            double price_;
            double volume_;
            char category_;
            char type_;
            char price_denom_code_;
    };

    struct long_trade_message
    {
        static const size_t SIZE = 82;
        static const size_t FIELDS = 23;
        static const size_t RESTRICTION_SIZE = 3;

        enum
        {
            MESSAGE_HEADER,
            SECURITY_SYMBOL,
            TEMPORARY_SUFFIX,
            TEST_MESSAGE_INDICATOR,
            TRADE_REPORTING_FACILITY_IDENTIFIER,
            PRIMARY_LISTING_MARKET_PARTICIPANT_IDENTIFIER,
            RESERVED,
            FINANCIAL_STATUS,
            CURRENCY_INDICATOR,
            HELD_TRADE_INDICATOR,
            INSTRUMENT_TYPE,
            SELLERS_SALE_DAYS,
            SALE_CONDITION,
            TRADE_THROUGH_EXEMPT_INDICATOR,
            SHORT_SALE_RESTRICTION_INDICATOR,
            RESERVED_2,
            PRICE_DENOMINATOR_INDICATOR,
            TRADE_PRICE,
            TRADE_VOLUME,
            CONSOLIDATED_HIGH_LOW_LAST_INDICATOR,
            PARTICIPANT_OPEN_HIGH_LOW_LAST_INDICATOR,
            RESERVED_3,
            STOP_STOCK_INDICATOR
        };
        static const int LENGTH[FIELDS];
        static const int OFFSET[FIELDS];

        static const char RESTRICTION_TYPE[RESTRICTION_SIZE];
        static const char RESTRICTION_CATEGORY[RESTRICTION_SIZE];
    };

    struct short_trade_message
    {
        static const int SIZE = 44;
        static const size_t FIELDS= 9;
        static const size_t RESTRICTION_SIZE = 2;

        enum
        {
            MESSAGE_HEADER,
            SECURITY_SYMBOL,
            SALE_CONDITION,
            TRADE_VOLUME,
            PRICE_DENOMINATOR_INDICATOR,
            TRADE_PRICE,
            CONSOLIDATED_HIGH_LOW_LAST_INDICATOR,
            PARTICIPANT_OPEN_HIGH_LOW_LAST_INDICATOR,
            RESERVED
        };
        static const int LENGTH[FIELDS];
        static const int OFFSET[FIELDS];

        static const char RESTRICTION_TYPE[RESTRICTION_SIZE];
        static const char RESTRICTION_CATEGORY[RESTRICTION_SIZE];
    };

    typedef boost::shared_ptr< trade_message > trade_message_ptr;

}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
