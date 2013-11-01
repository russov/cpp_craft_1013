#include "solution.h"
#include <limits>
#include <boost/thread.hpp>
#include <stdexcept>

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


void task4_5::solution::calc_proc(task4_5::data_iterator<int>& it, size_t step)
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
    min_ = std::min(min, min_);
    max_ = std::max(max, max_);

}

void task4_5::solution::calculate()
{
    if(data_.empty())
    {
        return;
    }
    min_ = std::numeric_limits< int >().max();
    max_ = std::numeric_limits< int >().min();

    boost::thread_group threads;

    uint32_t max_threads = boost::thread::hardware_concurrency();
    uint32_t n = boost::thread::hardware_concurrency();
    while(n--)
    {
        threads.create_thread(
            boost::bind(&task4_5::solution::calc_proc, this,
            task4_5::data_iterator<int>(data_, n), max_threads));

    }
    threads.join_all();
    need_calculate_ = false;
}
