#include "solution.h"

task4_6::solution::solution( const lines& calulator_lines )
{
}

int task4_6::solution::result_for( const std::string& key ) const
{
	return 0;
}


const task4_6::lines task4_6::solution::GetPolishReversString(const task4_6::lines& expressions)
{
	std::stack<char> op_stack;
	std::stringstream revers_polish;
	
	char current_symbol;
	for(int i = 0; i < expression.size(); i++)
	{
		current_symbol = expression[i];
		if(IsNumber(current_symbol)||std::isalpha(static_cast<int>(current_symbol)))
		{
			revers_polish << current_symbol; 
		}
		else
		{
			if(current_symbol == '(')
			{
				op_stack.push(current_symbol);
			}
			else if(IsOperation(static_cast<int>(current_symbol)))
			{
				revers_polish << " ";
				if(!op_stack.empty())
				{
					if( OpPrior(op_stack.top()) > OpPrior(current_symbol))
					{
						revers_polish << op_stack.top() << " ";
						op_stack.pop();
					}
				}
				op_stack.push(current_symbol);

			}
			else if(current_symbol == ')')
			{
				while(op_stack.top() != '(')
				{
					revers_polish << " " << op_stack.top();
					op_stack.pop();
				}
			}
		}
	}
	revers_polish << " ";
	while(!op_stack.empty())
	{
		revers_polish << op_stack.top() << " ";
		op_stack.pop();
	}
	return revers_polish.str();

}
void task4_6::solution::CalculatePolishExpression(const task4_6::lines& polish_expressions)
{
	double result = 0.0;
	std::string value;
	double value1, value2;
	std::stringstream ss_in(polish_expression);
	std::stack<double> values_stack;

	while(ss_in >> value)
	{
		if(IsOperation(value[0]))
		{
			value2 =  values_stack.top();
			values_stack.pop();
			value1 =  values_stack.top();
			values_stack.pop();
			switch (value[0])
			{
			case '+':
				result = value1 + value2;
				break;
			case '-':
				result = value1 - value2;
				break;
			case '*':
				result = value1 * value2;
				break;
			case '/':
				if(value2 == 0.0)
				{
					throw std::logic_error("zero div MUST ADD LINE");
				}
				result = value1 / value2;
				break;
			default:
				break;
			}
			values_stack.push(result);
		}
		else
		{
			values_stack.push(boost::lexical_cast<double>(value));
		}
	}
	

	return result;
}

bool task4_6::solution::is_number(char ch)
{
	return (std::isdigit(ch) || ch == '.');
}

bool task4_6::solution::is_operation(char ch)
{
	if((ch == '*')||(ch == '+')||(ch == '-')||(ch == '/'))
		return true;
	else
		return false;
}

uint32_t task4_6::solution::operation_priority(char ch)
{
	if((ch == '+')||(ch == '-'))
	{
		return 1;
	}else if((ch == '/')||(ch == '*'))
	{
		return 2;
	}
	return 0;
}
