#include "solution.h"


task4_5::solution::solution( const data_type& data )
{
	a=data;
}

void task4_5::solution::count_min(std::vector< int > &b) const
{	
	int Min = (1<<31)-1;
	for(int i=0;i<b.size();i++)
		Min = std :: min(Min,b[i]);
	{
		boost :: mutex :: scoped_lock lock(mtx);
		MIN = std :: min(Min,MIN);
	}
}

void task4_5::solution::flows_to_find_min() const
{
	if (a.size()==0) 
		{
			MIN = 0;
			return;
		}
	boost :: thread_group group;
	for(size_t i=0;i<a.size();i++)
		group.create_thread(boost :: bind (&task4_5 :: solution :: count_min,this,a[i]));	
	group.join_all();	
}

void task4_5::solution::count_max(std::vector< int > &b) const
{	
	int Max = -(1<<31);
	for(int i=0;i<b.size();i++)
		Max = std :: max(Max,b[i]);
	{
		boost :: mutex :: scoped_lock lock(mtx);
		MAX = std :: max(MAX, Max);
	}
}

void task4_5::solution::flows_to_find_max() const
{
	if (a.size()==0) 
		{
			MAX = 0;
			return;
		}
	boost :: thread_group group;
	for(size_t i=0;i<a.size();i++)
		group.create_thread(boost :: bind (&task4_5 :: solution :: count_max,this,a[i]));	
	group.join_all();	
}


int task4_5::solution::get_min() const
{
	MIN = (1<<31)-1;
	flows_to_find_min();
	return MIN;
}
int task4_5::solution::get_max() const
{
	MAX = -(1<<31);
	flows_to_find_max();
	return MAX;
}
