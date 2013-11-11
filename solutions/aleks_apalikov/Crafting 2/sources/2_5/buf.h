#pragma once
//solution of task 2.5
#include "DealsElem.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

class buf
{
	const unsigned int TotalTypes;
	vector<size_t> numBytes; //used number of Bytes of idx type
	vector<size_t> totalBytes; //total used Bytes
	fstream in;
	ofstream out;
	bool fileErr;
public:
	buf(string & fileName );
	int createOutput(); //read elements one by one and output average values
public:
	~buf(void);
};
