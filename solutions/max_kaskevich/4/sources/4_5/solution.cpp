#include "solution.h"
#include <limits>
#include <boost/thread.hpp>
#include <stdexcept>

static const uint32_t max_threads = 10u;
static const uint32_t pref_min_elems_for_thread = 10u;


task4_5::solution::solution( const data_type& data ) :
    data_(data),
    min_(0),
    max_(0),
    need_calculate_(true)
{
}

int task4_5::solution::get_min() const
{
    if(need_calculate_) 
    {
        const_cast<solution*>(this)->calculate();
    }
	return min_;
}
int task4_5::solution::get_max() const
{
    if(need_calculate_) 
    {
        const_cast<solution*>(this)->calculate();
    }
    return max_;
}


void task4_5::solution::calc_proc(task4_5::data_iterator<int> it, size_t step)
{
    int min = std::numeric_limits< int >().max();
    int max = std::numeric_limits< int >().min();

    while(!it.is_end())
    {
        min = std::min( min, *it );
        max = std::max( max, *it );
        it += step;
    }

    boost::lock_guard<boost::mutex> lock(mtx_);

}

void task4_5::solution::calculate()
{
    if(data_.empty())
    {
        return;
    }








    min_ = std::numeric_limits< int >().max();
    max_ = std::numeric_limits< int >().min();

    uint64_t global_size =  0;

    for(auto& subdata: data_)
    {
        global_size += subdata.size();
    }
    uint64_t size_for_every_thread = global_size / max_threads + 1;


    for(auto& subdata: data_)
    {
        for(auto& elem: subdata)
        {
            min_ = std::min( min_, elem );
            max_ = std::max( max_, elem );
        }
    }
    need_calculate_ = false;
}
