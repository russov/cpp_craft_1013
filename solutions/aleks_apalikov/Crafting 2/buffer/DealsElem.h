#pragma once
#include <basetsd.h>

struct DealsElem
{
	UINT32 Type;
	UINT32 Time;
	UINT32 Len;
private:
	char* Msg;
	DealsElem(void);
	DealsElem(const DealsElem& de);
	void operator=(const DealsElem& de);

public:
	DealsElem(UINT32 type, UINT32 time, UINT32 len, char* str);
	~DealsElem(void);
};
enum Types {
	MARKET_OPEN = 1u,
	TRADE = 2u,
	QUOTE = 3u,
	MARKET_CLOSE = 4u
};
