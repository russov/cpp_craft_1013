#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <iterator>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution
	{
        static const uint32_t max_threads = 10u;
        static const uint32_t pref_min_elems_for_thread = 10u;
        const data_type& data_;
        int min_;
        int max_;
        bool need_calculate_;

        int operator[](size_t index);

        void calculate();
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};

    template <typename T>
    class data_iterator : public std::iterator<std::forward_iterator_tag, T>
    {
         const data_type& data_;
         data_type::const_iterator it_;
         size_t index_;

    public:
        data_iterator( const data_type& data, uint64_t index)
        {
            it = data_.cbegin();
            if (it == data_.cend())
            {
                throw std::out_of_range("empty data");
            }
            while (index >= it->size())
            {

                index -= it->size();
                ++it;
                if (it == data_.cend())
                {
                    throw std::out_of_range("out of range");
                }
            }        
        }
    };
}

#endif // _TASK4_5_SOLUTION_H_
