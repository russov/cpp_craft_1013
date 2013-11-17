#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
	private :
		struct Node_
		{
			T val_;
			Node* next_;

			Node_()
			{
				val_ = T();
				next_ = NULL;
			}
		};

		mutable boost::mutex queue_;
		mutable boost::mutex empty_;
		boost::condition wait_not_empty_;

		Node_* begin_;
		Node_* end_;

	public :
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
		begin_ = new Node_();
		end_ = begin_;
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
		Node_* cur = begin_;
		
		while ( cur != end_ )
		{
			begin_ = begin_->next_;
			delete cur;
			cur = begin_;
		}

		delete cur;
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& el )
	{
		std::memcpy( &end->val_, &el, sizeof(T) );

		end_->next = new Node();
		end_ = end_->next;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& poped_el )
	{
		if ( this->empty() )
		{
			return true;
		}
		else 
		{
			memcpy( &poped_el, &(begin->val), sizeof(T) );
			Node_* poped = begin_;
			begin_ = begin_->next_;

			delete cur;
			return false;
		}
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		return begin_ == end_;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		Node_* tracker = begin_;
		
		size_t size = 0;

		while( tracker->next )
		{
			size++;
			tracker = tracker->next;
		}
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
