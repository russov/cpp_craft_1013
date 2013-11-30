#include <vector>
#include <utility>

#include <signal.h>

#include <boost/thread.hpp>

#include "market_data_receiver.h"
#include "market_data_processor.h"

#include "property.h"

using namespace multicast_communication;

static market_data_processor_impl processor; 
static boost::condition_variable cond_var; 
static boost::mutex ctrl_c; 

static void sighandler(int sig)
{
    processor.dump(SOURCE_DIR "/market_data.dat");
    cond_var.notify_one();
}

int main()
{ 
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler); 

    property::reader reader(SOURCE_DIR "/config.ini");
    if(reader.opened())
    { 
        size_t trade_threads_size = reader.trade_thread_size();
        size_t quote_threads_size  = reader.quote_thread_size();

        std::vector<std::pair<std::string, unsigned short> > const& trades = reader.trades();
        std::vector<std::pair<std::string, unsigned short> > const& quotes = reader.quotes(); 

        market_data_receiver receiver(trade_threads_size, quote_threads_size, trades, quotes,  processor );
        receiver.start(); 

        boost::mutex::scoped_lock lock(ctrl_c);
        cond_var.wait(lock); 
        receiver.stop();
    } else 
    {
        std::cerr << "Unable to open config file at " SOURCE_DIR " /config.ini" << std::endl;
    }
}
