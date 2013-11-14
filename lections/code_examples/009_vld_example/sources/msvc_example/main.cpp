#define _CRTDBG_MAP_ALLOC defined

#include <stdlib.h>
#include <crtdbg.h>

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
//		delete ptr_;
	}
};

int main()
{

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );

	_CrtDumpMemoryLeaks();
	{
		bad_string bs1;
		bad_string bs2;
	}

//	_CrtMemState s1, s2, s3;
//	_CrtMemCheckpoint( &s1 );
	
	{
		bad_string bs3;
	}

//	_CrtMemCheckpoint( &s2 );

//	if ( _CrtMemDifference( &s3, &s1, &s2) )
//		_CrtMemDumpStatistics( &s3 );

	{
		bad_string bs4;
		bad_string bs5;
	}

}
