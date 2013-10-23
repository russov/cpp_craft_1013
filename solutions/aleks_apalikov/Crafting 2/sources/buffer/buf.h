#pragma once
//solution of task 2.5
#include "DealsElem.h"

class buf
{
	fstream in;
	ofstream out;
	bool fileErr;
public:
	buf(string & fName);
	int createOutput(); //read elements one by one and output average values
public:
	~buf(void);
};
