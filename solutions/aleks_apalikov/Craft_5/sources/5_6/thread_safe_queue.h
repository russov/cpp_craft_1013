#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
	public:
		boost::mutex protector;
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
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  )
	{
		boost::mutex::scoped_lock push_lock(protector);
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& )
	{
		boost::mutex::scoped_lock pop_lock(protector);
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		return false;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		return 0ul;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
