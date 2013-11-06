#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>

namespace task4_6
{
    enum TokenValue
    {
        END,
        NAME,
        NUMBER,
        PLUS='+',
        MINUS='-',
        MUL='*',
        DIV='/',
        ASSIGN='=',
        LP='(',
        RP=')'
    };

	typedef std::vector< std::string > lines;

	class solution
	{
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
    private:
        TokenValue GetToken();

        TokenValue cur_tok;
	};
}

#endif // _TASK4_5_SOLUTION_H_