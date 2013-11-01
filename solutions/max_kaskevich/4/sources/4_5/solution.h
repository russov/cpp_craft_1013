#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <iterator>
#include <boost/thread.hpp>


namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

    template <typename T>
    class data_iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        const data_type& data_;
        data_type::const_iterator it_;
        size_t index_;
        bool is_end_;
    public:
        data_iterator( const data_type& data, size_t index) :
            data_(data),
            index_(0),
            is_end_(false)
        {
            it_ = data_.cbegin();
            operator+=(index); 
        }

        bool is_end()
        {
            return is_end_;
        }

        T operator *()
        {
            return (*it_)[index_];
        }

        void operator+=(size_t x)
        {
            if(is_end_)
            {
                return;
            }
            index_ += x;
            while(index_ >= it_->size())
            {
                index_ = index_ - it_->size();
                ++it_;
                if(it_ == data_.end())
                {
                    is_end_ = true;
                    return;
                }

            }
        }

        data_iterator& operator++()
        {
            += 1;
            return *this;
        }

    };

	class solution
	{
        const data_type& data_;
        int min_;
        int max_;
        bool need_calculate_;

        boost::mutex mtx_;

        void calc_proc(task4_5::data_iterator<int>& it, size_t step);
        void calculate();
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};



}

#endif // _TASK4_5_SOLUTION_H_
