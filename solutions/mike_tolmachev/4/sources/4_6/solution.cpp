#include "solution.h"

task4_6::solution::solution( const lines& calulator_lines )
{
    for (auto it = calulator_lines.begin(); it != calulator_lines.end(); ++it)
    {
        str.clear();
        str.str(*it);
        Expr(true);
    }
}

int task4_6::solution::result_for( const std::string& key ) const
{
    int res = 0;
    auto it = names.find(key);
    if (it != names.end())
    {
        res = it->second;
    }

	return res;
}

task4_6::TokenValue task4_6::solution::GetToken()
{
    char ch = 0;

    do
    {
        str.get(ch);
        if (str.bad())
        {
            ch = 0;
        }
    } while (isspace(ch) && str.good());

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
            str.putback(ch);
            str >> number_val;
            cur_tok = NUMBER;
        }
        else if (isalpha(ch))
        {
            name_val.clear();
            while(str.good() && isalnum(ch))
            {
                name_val.push_back(ch);
                str.get(ch);
            }
            str.putback(ch);
            cur_tok = NAME;
        }
    }

    return cur_tok;
}

double task4_6::solution::Expr(bool get)
{
    double left = Term(get);

    while (true)
    {
        switch (cur_tok)
        {
        case PLUS:
            left += Term(true);
            break;
        case MINUS:
            left -= Term(true);
            break;
        default:
            return left;
        }
    }
}

double task4_6::solution::Term(bool get)
{
    double left = Prim(get);

    while (true)
    {
        switch(cur_tok)
        {
        case MUL:
            left *= Prim(true);
            break;
        case DIV:
            if (double d = Prim(true))
            {
                left /= d;
                break;
            }
        default:
            return left;
        }
    }
}

double task4_6::solution::Prim(bool get)
{
    if (get)
    {
        GetToken();
    }

    switch (cur_tok)
    {
    case NUMBER:
        {
            double v = number_val;
            GetToken();
            return v;
        }
    case NAME:
        {
            std::string cur_name = name_val;
            auto it = names.find(cur_name);
            if (it == names.end())
            {
                if (GetToken() == ASSIGN)
                {
                    double v2 = Expr(true);
                    names[cur_name] = v2 > 0 ? v2 + 0.5 : v2 - 0.5;
                    return names[cur_name];
                }
                else
                {
                    throw "'B' variable not defined at line %1%";
                }
            }

            GetToken();
            return it->second;
        }
    case MINUS:
        return -Prim(true);
    case LP:
        {
            double e = Expr(true);
            //if (cur_tok != RP)
            GetToken();
            return e;
        }
    default:
        return 0;
    }
}
