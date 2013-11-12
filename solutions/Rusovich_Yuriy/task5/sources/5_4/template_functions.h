#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
	template< bool delete_first, typename Container >
	void clear_container( Container& c)
	{
		if (delete_first)
		{
			for (size_t i = 0; i != c.size(); ++i)
			//for (Container::iterator it = c.begin(); it != c.end(); ++it)
			{
				//int *temp = reinterpret_cast < int *>(c[i]);
				//delete temp;
			}
		}


		c.clear();
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& c)
	{
		c.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

