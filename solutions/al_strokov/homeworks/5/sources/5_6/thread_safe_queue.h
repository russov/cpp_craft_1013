#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

//dbg
#include <iostream>

#include <cstdlib>
#include <queue>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
	private:
		std::queue<T> q_;
	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );
		bool pop();

		bool empty() const;
		size_t size() const;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
	{
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  value)
	{
		q_.push(value);
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& value )
	{
		if(!q_.empty()){
			value = q_.front();
			return true;
		}
		return false;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop()
	{
		if(!q_.empty()){
			q_.pop();
			return true;
		}
		return false;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		return q_.empty();
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		return q_.size();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
