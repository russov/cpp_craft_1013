#include "vld.h"

class bad_string
{
	char *ptr_;
public:
	explicit bad_string()
	{
		ptr_ = new char[ 14 ];
	}
	~bad_string()
	{
		delete ptr_;
	}
};

int main()
{

	{
		bad_string bs1;
		bad_string bs2;
	}

	
	{
		bad_string bs3;
	}

	{
		bad_string bs4;
		bad_string bs5;
	}

}
