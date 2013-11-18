#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>
#include <queue>
#include <deque>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
		std::deque<T> deq;
		std::queue<T> que;
	public:
		boost::mutex protector;
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() ;
		size_t size() ;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
	{
		boost::mutex::scoped_lock constructor(protector);
		que = std::queue<T>(deq);
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& new_element )
	{
		boost::mutex::scoped_lock push_lock(protector);
		que.push(new_element);
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result )
	{
		boost::mutex::scoped_lock pop_lock(protector);
		if(que.empty())
			return false;
		result = que.front();
		que.pop();
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() 
	{
		boost::mutex::scoped_lock push_lock(protector);
		return que.empty();
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() 
	{
		boost::mutex::scoped_lock push_lock(protector);
		return que.size();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
