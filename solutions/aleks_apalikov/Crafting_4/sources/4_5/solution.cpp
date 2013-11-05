#include "solution.h"
#include <limits>

task4_5::solution::solution( const data_type& data )
{
	thread_amount = data.size();
	this->data_ = data;
	minix = std::numeric_limits<int>::min();
	dMin = 1000;
	dMax = minix;
	results.reserve(data_.size());
	results.assign(data_.size(), 1000);

	resultsMax.reserve(data_.size());
	resultsMax.assign(data_.size(), minix);
}

int task4_5::solution::get_min() 
{
	for( size_t i = 0 ; i < thread_amount ; ++i )
	{
		min_routine mini(std::numeric_limits<int>::max(), data_[i]);
		threads_.create_thread(mini) ;
	}
	return 0;
}
int task4_5::solution::get_max() const
{
	return 0;
}
/*
void task4_5::solution::max_routine( data_vector & vec, int & val )
{
	val = mini;
	for(data_vector::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if(*it > dMax)
		{
			dMax = *it;
		}
	}
}*/
