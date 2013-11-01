#include "solution.h"

task4_5::solution::solution( const data_type& data )
{
    boost::thread_group thread_group;
    for(int i = 0; i < 4; ++i)
    {
        thread_group.create_thread(boost::bind(&solution::thread_fun, this));
    }

    thread_group.join_all();
}

int task4_5::solution::get_min() const
{
	return 0;
}
int task4_5::solution::get_max() const
{
	return 0;
}
task4_5::data_type::const_iterator task4_5::solution::GetVector()
{
}
void task4_5::solution::thread_fun()
{
    auto it = GetVector();

    std::max_element(it->begin(), it->end());
}