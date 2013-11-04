#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <stack>
#include <cctype>
#include <stdint.h>
#include <sstream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

namespace task4_6
{
	typedef std::vector< std::string > lines;

	class solution
	{
	const lines GetPolishReversString(const lines& expressions);
	void CalculatePolishExpression(const lines& polish_expressions);
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	};
}

#endif // _TASK4_6_SOLUTION_H_