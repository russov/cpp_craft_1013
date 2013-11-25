#include "buf.h"


buf::buf( string & fileName ):TotalTypes(100000)
{
	fileErr = false;
	string fileN1 =  fileName + ".in"; 
	in.open(fileN1.c_str(), fstream::in | fstream::binary);
	string fileN2 = fileName + ".out"; 
	out.open(fileN2.c_str(), fstream::out | fstream::binary);
	if(!in.is_open() || !out.is_open())
	{
		cout << "File not found! "<<endl;
		fileErr = true;
		return;
	}
	numBytes.resize(TotalTypes + 1, 0);
	totalBytes.resize(TotalTypes + 1, 0);

}

buf::~buf(void)
{

	in.close();
	out.close();
}


int buf::createOutput()
{
	if (fileErr)
	{
		return -1;
	}
	int n = 0; //total number of objects
	UINT32 time, len, type;
	UINT32 curTime = 0;
	char* str = NULL;
	bool toWrite = false;
	DealsElem* de = NULL;
	while (in)
	{
		type = read_uint32(in);
		time = read_uint32(in);
		if(time>curTime)
		{
			curTime = time;
			for(vector<size_t>::iterator it = numBytes.begin(), tb = totalBytes.begin(); it != numBytes.end(); it++, tb++)
			{
				*tb += *it;
			}
			numBytes.assign(TotalTypes + 1, 0);
		}		
		len = read_uint32(in);
		if ((numBytes[type] + len + 3 * sizeof(UINT32)) <= 2400)
		{
			numBytes[type] += len + 3 * sizeof(UINT32);
		}
		if( in.peek() == EOF )
		{
			break;
		}
		if(len == 0)
			continue;
		str = new char[len+1];
		for (size_t i = 0; i < len; i++)
		{
			in.read(&(str[i]), 1);
		}
		str[len] = '\0';
		n++;
		if(str)
		{
			delete[] str;
		}
	}
	for(vector<size_t>::iterator it = numBytes.begin(), tb = totalBytes.begin(); it != numBytes.end(); ++it, ++tb)
	{
		*tb += *it;
	}
	type = 0;
	for(vector<size_t>::iterator tb = totalBytes.begin(); tb != totalBytes.end(); ++tb)
	{
		if (*tb == 0)
		{
			type++;
			continue;
		}
		if (de)
		{
			delete de;
		}

		write_uint32(out, type);
		write_double(out, (double)*tb/curTime);
		type++;
	}
	return n;
}
