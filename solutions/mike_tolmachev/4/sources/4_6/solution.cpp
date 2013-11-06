#include "solution.h"

task4_6::solution::solution( const lines& calulator_lines )
{
}

int task4_6::solution::result_for( const std::string& key ) const
{
	return 0;
}

task4_6::TokenValue task4_6::solution::GetToken()
{
    char ch = 0;

    //getch

    switch (ch)
    {
    case 0:
        cur_tok = END;
        break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '(':
    case ')':
        cur_tok = TokenValue(ch);
        break;
    default:
        if (isdigit(ch))
        {
            //todo
            cur_tok = NUMBER;
        }
        else if (isalpha(ch))
        {
            //todo
            cur_tok = NAME;
        }
    }

    return cur_tok;
}
