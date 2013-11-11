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
	Msg = new char[strlen(str)+1];
	strcpy(Msg, str);

}

DealsElem::~DealsElem(void)
{
	delete[] Msg;
}

UINT32 write_uint32( ofstream& ins, UINT32 value)
{
	for (unsigned size = 0; size < sizeof( UINT32 ); ++size)
		ins.put(value << (8 * size));
	return value;
}

void DealsElem::operator<<(ofstream& out)
{
	write_uint32(out, this->Type);
	write_uint32(out, this->Time);
	write_uint32(out, this->Len);
	write_str(out, this->Msg);
//	return out;
}

UINT32 read_uint32( fstream& ins)
{
	UINT32 value = 0;
	for (unsigned size = 0; size < sizeof( UINT32 ); ++size)
		value |= ins.get() << (8 * size);
	return value;
}

char* write_str( ofstream& ins, char* str )
{

	for (unsigned size = 0; size < strlen(str); ++size)
		ins.put(str[size]);
	return str;

}

ofstream& operator<<( ofstream& out, const DealsElem& de )
{
	write_uint32(out, de.Type);
	write_uint32(out, de.Time);
	write_uint32(out, de.Len);
	write_str(out, de.Msg);
	return out;
}

