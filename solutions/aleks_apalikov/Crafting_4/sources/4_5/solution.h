#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;
	typedef std::vector< int > data_vector;
	class min_routine
	{
	private:
		int val;
		std::vector<int>& arr;
	public:
		min_routine(int v, std::vector<int>& a):val(v),arr(a)
		{
		}
		int get()
		{
			return val;
		}
		void operator()()
		{
			for(data_vector::iterator it = arr.begin(); it != arr.end(); it++)
			{
				if(*it < val)
				{
					val = *it;
				}
			}
		}
	};

	class solution
	{
	private:
		data_type data_;
		data_vector results;

		data_vector resultsMax;
		boost::thread_group threads_;
		size_t thread_amount;
	public:
		static int minix;
		mutable boost::mutex min_mut;
		int dMin;
		mutable boost::mutex max_mut;
		int dMax;
//		void min_routine(data_vector & vec, int & val);
//		void max_routine(data_vector & vec, int & val);
		explicit solution( const data_type& data );
		int get_min();
		int get_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_