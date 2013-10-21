#include "Deals.h"

Deals::Deals( string & fileName )
{
	fileErr = false;
	string fileN1 = fileName + ".in"; 
	in.open(fileN1.c_str(), ios_base::in | ios_base::binary);
	string fileN2 = fileName + ".in"; 
	out.open(fileN2.c_str(), ios_base::out | ios_base::binary);
	if(!in.is_open() || !out.is_open())
	{
		cout << "File not found! "<<endl;
		fileErr = true;
		return;
	}


}

Deals::~Deals(void)
{
	in.close();
	out.close();
}

int Deals::createOutput()
{
	if (fileErr)
	{
		return -1;
	}
	int n = 0; //total number of objects
	UINT32 type, time, len, currentTime = 0;
	char* str;
	bool toWrite = false;
	DealsElem* de;
	while (!in.eof())
	{
		in >> type;
		toWrite = (type>=MARKET_OPEN && type<=MARKET_CLOSE);
		in >> time;
		if(currentTime != 0) //test of first read
		{
			if (time<=currentTime-2)
			{
				toWrite |= false;
			}
			else 
			{
				toWrite |= true;
				currentTime = time;
			}

		}
		else
		{
			toWrite |= true;
			currentTime = time;
		}
		in >> len;
		if(str)
		{
			delete[] str;
		}
		if (de)
		{
			delete de;
		}
		str = new char[len+1];
		for (int i = 0; i < len; i++)
		{
			in >> str[i];
		}
		str[len] = '\0';
		n++;
		de = new DealsElem(type, time, len, str);
		if (toWrite)
		{
			out << de;
		}
	}
	return n;
}

