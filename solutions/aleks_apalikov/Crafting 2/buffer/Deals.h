#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "DealsElem.h"

using namespace std;
class Deals
{
	fstream in;
	fstream out;
	bool fileErr;
public:
	Deals(string & fileName);
	int createOutput(); //read one element and output if condtions are true
	~Deals(void);
};
