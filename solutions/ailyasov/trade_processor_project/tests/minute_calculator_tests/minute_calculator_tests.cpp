#include <vector>
#include <string>
#include <random>
#include <algorithm>

#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "test_registrator.h"
#include "minute_calculator.h"
#include "trade_message.h"
#include "quote_message.h"

namespace minute_calculator
{
    namespace tests_
    {
        typedef std::vector<char> message_type; 
        typedef boost::shared_ptr<trade_message> trade_message_ptr;

        static message_type message(std::string const& s)
        {
            return message_type(s.begin(), s.end());
        }
        static trade_message_ptr t_m(std::string const& s, std::string ts, double price, double volume)
        {
            trade_message_ptr t = boost::make_shared<trade_message>(message(""));
            t->security_symbol_ = s;
            t->price_ = price;
            t->volume_ = volume;
            t->timestamp_ = ts;
            return t;
        }
        static quote_message_ptr q_m(std::string const& s
                , std::string const& ts
                , double bid_price
                , double bid_size
                , double offer_price
                , double offer_size)
        {
            quote_message_ptr q = boost::make_shared<quote_message>(message(""));

            q->timestamp_ = ts;
            q->security_symbol_ = s;
            q->bid_price_ = bid_price;
            q->bid_size_ = bid_size;
            q->offer_price_ = offer_price;
            q->offer_size_ = offer_size;

            return q;
        }

        void minute_calculator_tests()
        {
            {
                BOOST_CHECK_NO_THROW ();
                BOOST_CHECK_EQUAL(0, 0);
            }
            {
                boost::uint32_t m = calculator::minute_from_timestamp("12");
                BOOST_CHECK_EQUAL(62, m);
            }
            {
                thread_safe_queue< trade_message_ptr > trades;
                trades.push(t_m("APPLE", "123345", 5.6, 7.8));
                trades.push(t_m("GOOGLE","123345", 5.7, 9.99));
                trades.push(t_m("GOOGLE","123456", 5.5, 13.1));
                trades.push(t_m("GOOGLE","123456", 5.9, 13.1));
                trades.push(t_m("GOOGLE","123456", 5.6, 13.1));
                trades.push(t_m("GOOGLE","133456", -123, 13.1));
                thread_safe_queue< quote_message_ptr > quotes;
                quotes.push(q_m("GOOGLE","123456", 1.1, 2.2, 3.3, 4.4));
                quotes.push(q_m("GOOGLE","123456", 5.5, 0, 6.6, 0.0));
                quotes.push(q_m("APPLE", "123456", 5.7, 0.9, 6.4, -4.5));
                calculator calc(trades, quotes);
                calc.start(); 
                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                calc.stop(); 
                datafeeds_type datafeeds = calc.get_minute_datafeed(); 
                BOOST_CHECK_EQUAL(2, datafeeds.size());
                std::vector< minute_datafeed_ptr> apple = datafeeds["APPLE"];
                std::vector< minute_datafeed_ptr> google = datafeeds["GOOGLE"];

                BOOST_CHECK_EQUAL(1, apple.size());
                BOOST_CHECK_EQUAL(2, google.size());

                BOOST_CHECK_EQUAL("APPLE", apple[0]->stock_name_);
                BOOST_CHECK_EQUAL(5.6, apple[0]->open_price_);
                BOOST_CHECK_EQUAL(5.6, apple[0]->high_price_);
                BOOST_CHECK_EQUAL(5.6, apple[0]->low_price_);
                BOOST_CHECK_EQUAL(5.6, apple[0]->close_price_);
                BOOST_CHECK_EQUAL(7.8, apple[0]->volume_);
                BOOST_CHECK_EQUAL(0.9, apple[0]->bid_);
                BOOST_CHECK_EQUAL(-4.5, apple[0]->ask_);

                BOOST_CHECK_EQUAL("GOOGLE", google[0]->stock_name_);
                BOOST_CHECK_EQUAL(5.7, google[0]->open_price_);
                BOOST_CHECK_EQUAL(5.9, google[0]->high_price_);
                BOOST_CHECK_EQUAL(5.5, google[0]->low_price_);
                BOOST_CHECK_EQUAL(5.6, google[0]->close_price_);
                BOOST_CHECK_EQUAL(49.29, google[0]->volume_);
                BOOST_CHECK_EQUAL(2.2, google[0]->bid_);
                BOOST_CHECK_EQUAL(4.4, google[0]->ask_);
            }
            {
                thread_safe_queue< trade_message_ptr > trades;
                double total_volume = 0;
                double volume = 0;
                double total_price = 0;
                double price;
                double open_price;
                double high_price;
                double low_price;
                double close_price;
                double bid;
                double total_bid;
                double ask;
                double total_ask;
                std::random_device rd;
                std::uniform_real_distribution<double> rand(0, 500.0);
                for(int i = 0; i < 1000; ++i)
                {
                    volume = rand(rd) + rand(rd);
                    price = -rand(rd) + i;
                    if(i == 0)
                        open_price = price;
                    if(i == 999)
                        close_price = price;
                    total_volume += volume;
                    total_price += price;
                    high_price = std::max(high_price, price);
                    low_price = std::min(low_price, price);
                    trades.push(t_m("APPLE", "123345", price, volume));
                    trades.push(t_m("GOOGLE", "123345", price, volume));
                }
                thread_safe_queue< quote_message_ptr > quotes;
                for(int i = 0; i < 1000; ++i)
                {
                    bid = rand(rd) + i;
                    total_bid += bid;
                    ask = rand(rd) - i;
                    total_ask += ask;
                    quotes.push(q_m("APPLE","123456", 100 + i, bid, 2000 + i, ask));
                    quotes.push(q_m("GOOGLE","123456", 100 + i, bid, 2000 + i, ask));
                }
                calculator calc(trades, quotes);
                calc.start(); 
                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                calc.stop(); 
                datafeeds_type datafeeds = calc.get_minute_datafeed(); 
                BOOST_CHECK_EQUAL(2, datafeeds.size());
                std::vector< minute_datafeed_ptr> apple = datafeeds["APPLE"];
                std::vector< minute_datafeed_ptr> google = datafeeds["GOOGLE"];

                BOOST_CHECK_EQUAL(1, apple.size());
                BOOST_CHECK_EQUAL(1, google.size()); 

                BOOST_CHECK_EQUAL("APPLE", apple[0]->stock_name_);
                BOOST_CHECK_EQUAL(open_price, apple[0]->open_price_);
                BOOST_CHECK_EQUAL(high_price, apple[0]->high_price_);
                BOOST_CHECK_EQUAL(low_price, apple[0]->low_price_);
                BOOST_CHECK_EQUAL(close_price, apple[0]->close_price_);
                BOOST_CHECK_EQUAL(total_volume, apple[0]->volume_);
                BOOST_CHECK_EQUAL(total_bid, apple[0]->bid_);
                BOOST_CHECK_EQUAL(total_ask, apple[0]->ask_);

                BOOST_CHECK_EQUAL("GOOGLE", google[0]->stock_name_);
                BOOST_CHECK_EQUAL(open_price, google[0]->open_price_);
                BOOST_CHECK_EQUAL(high_price, google[0]->high_price_);
                BOOST_CHECK_EQUAL(low_price, google[0]->low_price_);
                BOOST_CHECK_EQUAL(close_price, google[0]->close_price_);
                BOOST_CHECK_EQUAL(total_volume, google[0]->volume_);
                BOOST_CHECK_EQUAL(total_bid, google[0]->bid_);
                BOOST_CHECK_EQUAL(total_ask, google[0]->ask_);
            }
        }
    }
}
