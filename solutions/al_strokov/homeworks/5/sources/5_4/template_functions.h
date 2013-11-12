#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <iostream>

namespace task5_4
{
	template< bool delete_first, typename Container >
	void clear_container( Container& C)
	{
		std::cout << "C.size() == " << C.size() << std::endl;
		std::cout << delete_first << std::endl;
		if(!delete_first){
			C.clear();
		}else{
//			for(size_t i = 0 ; i < C.size(); i++){
//				delete C[i];
//			}
//			C.clear();
		}
	}

	template< bool delete_first, typename Container<*int> >
	void clear_container( Container& C){
		for(size_t i = 0 ; i < C.size(); i++){
			delete C[i];
		}
		C.clear();
	}


	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& )
	{
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

