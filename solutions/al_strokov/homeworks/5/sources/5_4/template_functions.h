#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <cstddef>
#include <set>
#include <list>

namespace task5_4
{
//	template< bool delete_first, typename Container >
//	void clear_container( Container& )
//	{
//	}

	template<typename Container, typename HelperValue>
	void helper_clear_container(Container& C, HelperValue v){
		C.clear();
	}

	template<typename Container, typename HelperValue>
	void helper_clear_container(Container& C, HelperValue* v){
		for(size_t i = 0; i < C.size(); i++){
			delete C[i];
		}
		C.clear();
	}

	template<typename Container>
	void clear_container(Container& C){
		if(C.size()){
			helper_clear_container(C, C[0]);
		}
	}

	template<typename ElementType>
	void clear_container(std::set<ElementType>& C){
		C.clear();
	}

	template<typename ElementType>
	void clear_container(std::set<ElementType*>& C){
		typename std::set<ElementType*>::iterator it;
		for (it = C.begin(); it != C.end(); ++it){
			delete *it;
		}
		C.clear();
	}

	template<typename ElementType>
	void clear_container(std::list<ElementType>& C){
		C.clear();
	}

	template<typename ElementType>
	void clear_container(std::list<ElementType*>& C){
		typename std::list<ElementType*>::iterator it;
		for (it = C.begin(); it != C.end(); ++it){
			delete *it;
		}
		C.clear();
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& )
	{
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

