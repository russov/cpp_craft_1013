#include "DealsElem.h"
#include <string.h>


DealsElem::DealsElem(void)
{
}

DealsElem::DealsElem( UINT32 type, UINT32 time, UINT32 len, char* str )
{
	Type = type;
	Time = time;
	Len = len;
	strcpy(Msg,str);

}

DealsElem::~DealsElem(void)
{
	delete Msg;
}
