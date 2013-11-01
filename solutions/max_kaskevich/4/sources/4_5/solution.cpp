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

int task4_5::solution::operator[](size_t index)
{
    data_type::const_iterator it = data_.cbegin();
    if (it == data_.cend())
    {
        throw std::out_of_range("empty data");
    }
    while (index >= it->size())
    {

        index -= it->size();
        ++it;
        if (it == data_.cend())
        {
            throw std::out_of_range("out of range");
        }
    }
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
    uint64_t size_for_every_thread = global_size / max_threads_ + 1;


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
