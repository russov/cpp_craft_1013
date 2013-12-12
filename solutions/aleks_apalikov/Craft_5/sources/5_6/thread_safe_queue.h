#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>
#include <queue>
#include <deque>

namespace task5_6
{
	typedef boost::mutex::scoped_lock lock_t;
	template< typename T >
	class thread_safe_queue
	{
		std::deque<T> deq;
		std::queue<T> que;
	public:
		mutable boost::mutex protector;
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
	{
		lock_t constructor(protector);
		que = std::queue<T>(deq);
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& new_element )
	{
		lock_t push_lock(protector);
		que.push(new_element);
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result )
	{
		lock_t pop_lock(protector);
		if(que.empty())
			return false;
		result = que.front();
		que.pop();
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		lock_t lock(protector);
		return que.empty();
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		lock_t lock(protector);
		return que.size();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
