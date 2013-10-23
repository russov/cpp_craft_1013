#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "DealsElem.h"
//solution of task 2.6

class Datafeed
{
	fstream in;
	ofstream out;
	bool fileErr;
public:
	Datafeed(string & fileName);
	int createOutput(); //read elements one by one and output average values
public:
	~Datafeed(void);
};
