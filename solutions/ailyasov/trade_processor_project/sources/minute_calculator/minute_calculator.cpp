#include <algorithm>
#include <utility>

#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/date_time/gregorian/gregorian.hpp> 
#include <ctime>

#include "minute_calculator.h" 

namespace minute_calculator
{
    calculator::calculator(thread_safe_queue< trade_message_ptr >& trades, 
            thread_safe_queue< quote_message_ptr >& quotes)
        : trades_(trades)
          , quotes_(quotes)
          , minute_t_(0)
          , minute_q_(0)
          , stop_(false) { }

    minute_datafeed::minute_datafeed()
        : minute_(0)
          , open_price_(0)
          , high_price_(0)
          , low_price_(0)  
          , close_price_(0)
          , volume_(0), bid_(0)
          , ask_(0) { } 

    calculator::~calculator()
    {
        try{ this->stop(); } catch(...) {}
    }
        
    static const double eps = 0.000001;

    void calculator::merge_trade_df(minute_datafeed_ptr m1, minute_datafeed_ptr m2)
    {
        BOOST_ASSERT((m1->open_price_ - 0.0) < eps);
        BOOST_ASSERT((m1->high_price_ - 0.0) < eps);
        BOOST_ASSERT((m1->low_price_ - 0.0) < eps);
        BOOST_ASSERT((m1->close_price_ - 0.0) < eps);
        BOOST_ASSERT((m1->volume_ - 0.0) < eps);

        m1->open_price_ = m2->open_price_;
        m1->high_price_ = m2->high_price_;
        m1->low_price_ = m2->low_price_;
        m1->close_price_ = m2->close_price_; 
        m1->volume_ = m2->volume_;
    }
    void calculator::merge_quote_df(minute_datafeed_ptr m1, minute_datafeed_ptr m2)
    {
        BOOST_ASSERT((m1->bid_ - 0.0) < eps);
        BOOST_ASSERT((m1->ask_ - 0.0) < eps);

        m1->bid_ = m2->bid_;
        m1->ask_ = m2->ask_;
    }

    void calculator::stop()
    {
        stop_ = true;
        trades_.stop();
        quotes_.stop(); 
        t_group_.join_all();
        next_minute(curr_t_, minute_t_, minute_t_ + 1, &calculator::merge_trade_df);
        next_minute(curr_q_, minute_q_, minute_q_ + 1, &calculator::merge_quote_df);
    }

    boost::uint32_t calculator::minute_from_timestamp(std::string const& s)
    { 
        boost::uint32_t minutes = 
            60 * (static_cast<boost::uint32_t>(s[0]) - '0') 
            + (static_cast<boost::uint32_t>(s[1]) - '0');
        return minutes;
    }
    boost::uint32_t calculator::minute_since_1900()
    {
        using namespace boost::gregorian;
        date d(1900, 1, 1); 
        date today = day_clock::local_day();
        days t = today - d;
        boost::uint32_t minutes = static_cast<boost::uint32_t>(t.days()) * 24 * 60; 
        return minutes;
    }

    //thread for processing trade messages
    //collects minute datafeed for trades until message for next minute is come
    //message with minute greater than current minute indicates next minute
    void calculator::process_trades()
    {
        while(!stop_)
        {
            boost::mutex::scoped_lock lock(protector_t_);
            trade_message_ptr t;
            trades_.wait_and_pop(t); 
            if(stop_)
            {
                break;
            }
            boost::uint32_t min = minute_from_timestamp(t->timestamp_) + minute_since_1900();
            next_minute(curr_t_, minute_t_, min, &calculator::merge_trade_df);
            std::map<std::string, minute_datafeed_ptr>::iterator it = curr_t_.find(t->security_symbol_);
            if(it == curr_t_.end())
            { 
                minute_datafeed_ptr p = boost::make_shared<minute_datafeed>();
                p->stock_name_ = t->security_symbol_;
                p->minute_ = min;
                p->open_price_ = t->price_;
                p->high_price_ = t->price_;
                p->low_price_ = t->price_;
                p->close_price_ = t->price_;
                p->volume_ = t->volume_;
                curr_t_.insert(std::make_pair(p->stock_name_, p));
            } else 
            {
                minute_datafeed_ptr p = it->second;
                if(p->high_price_ < t->price_)
                    p->high_price_ = t->price_;
                if(p->low_price_ > t->price_)
                    p->low_price_ = t->price_;
                p->volume_ += t->volume_; 
                p->close_price_ = t->price_;
            }
        }
    }

    //thread for processing quote messages
    //collects minute datafeed for quote until message for next minute is come
    //message with minute greater than current minute indicates next minute
    void calculator::process_quotes()
    {
        while(!stop_)
        {
            boost::mutex::scoped_lock lock(protector_q_);
            quote_message_ptr q;
            quotes_.wait_and_pop(q); 
            if(stop_)
            {
                break;
            }
            boost::uint32_t min = minute_from_timestamp(q->timestamp_) + minute_since_1900();
            next_minute(curr_q_, minute_q_, min, &calculator::merge_quote_df);
            std::map<std::string, minute_datafeed_ptr>::iterator i = curr_q_.find(q->security_symbol_);
            minute_datafeed_ptr p;
            if(i == curr_q_.end())
            {
                p = boost::make_shared<minute_datafeed>();
                p->stock_name_ = q->security_symbol_;
                p->minute_ = min;
                curr_q_.insert(std::make_pair(p->stock_name_, p));
            } else 
            {
                p = i->second;
            }
            p->bid_ += q->bid_size_; 
            p->ask_ += q->offer_size_;
        }
    }

    //updates to the next minute
    //merges trade and quote minute datafeeds if opposite one exists 
    //if not, put it to the collection
    //datafeeds are matched by stock_name and minute
    void calculator::next_minute(stock_df_& curr, boost::uint32_t& minute, boost::uint32_t const min, merge_function merge_func)
    {
        if(minute < min)
        {
            boost::mutex::scoped_lock lock(protector_datafeeds_);
            for(std::map<std::string, minute_datafeed_ptr>::iterator i = curr.begin(); i != curr.end(); ++i)
            {
                minute_datafeed_ptr i_df = i->second;
                std::vector<minute_datafeed_ptr> const& v = datafeeds_[i->first];
                bool exist = false;
                for(std::vector<minute_datafeed_ptr>::const_iterator j = v.begin(); j != v.end(); ++j)
                {
                    minute_datafeed_ptr j_df = *j;
                    if(i_df->stock_name_ == j_df->stock_name_ && i_df->minute_ == j_df->minute_)
                    { 
                        merge_func(j_df, i_df);
                        exist = true;
                        break;
                    }
                }
                if(!exist)
                    datafeeds_[i->first].push_back(i->second); 
            }
            curr.clear();
            minute = min;
        }
    }

    void calculator::start()
    { 
        t_group_.create_thread(boost::bind(&calculator::process_trades, this));

        t_group_.create_thread(boost::bind(&calculator::process_quotes, this));
    }

    datafeeds_type calculator::get_minute_datafeed()
    {
        boost::mutex::scoped_lock lock(protector_datafeeds_);
        return datafeeds_;
    } 

    std::ostream& operator<<(std::ostream& os, minute_datafeed const& d)
    {
        os << "[" << d.stock_name_ << " " << d.minute_ << " "
            " OPEN: " << d.open_price_ << 
            " HIGH: " << d.high_price_ << 
            " LOW: " << d.low_price_ <<
            " CLOSE: " << d.close_price_ << 
            " VOLUME: " << d.volume_ <<
            " BID: " << d.bid_ <<
            " ASK: " << d.ask_ <<
            "]" << std::endl;
        return os;
    }
}
