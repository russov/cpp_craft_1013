#pragma once
//solution of task 2.5
#include "DealsElem.h"
#include <fstream>
#include <string>
#include <iostream>
#include "DealsElem.h"
#include <vector>

class buf
{
	const int TotalTypes;
	vector<int> numBytes; //used number of Bytes of idx type
	vector<int> totalBytes; //total used Bytes
	fstream in;
	ofstream out;
	bool fileErr;
public:
	buf(string & fileName );
	int createOutput(); //read elements one by one and output average values
public:
	~buf(void);
};
