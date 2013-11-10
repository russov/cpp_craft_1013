#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <algorithm>
#include <iostream>

namespace task5_4
{
	std::equal_to< bool > should_delete;

	template <typename T>
	class delete_ptr
	{
		void operator()(T ptr)
		{
			delete ptr;
		}
	};

	template< bool delete_first, typename Container >
	void clear_container( Container& );

	template< typename Container >
	void clear_container< true, Container >( Container& container)
	{
		std::for_each( container.begin(), container.end(), 
			delete_ptr<Container::value_type>() );
		container.clear();
	}

	template< typename Container >
	void clear_container< false, Container >( Container& container)
	{
		container.clear();
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& container)
	{
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

