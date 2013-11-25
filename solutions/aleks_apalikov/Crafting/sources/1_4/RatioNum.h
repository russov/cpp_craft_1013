#pragma once
#include <fstream>
#include <vector>

using namespace std;
class RatioNum
{
	ifstream fs;
	ofstream of;
	int count;
	vector<float> codes;
	vector<float> passes;
public:
	RatioNum(const string& fileName = "Input2.txt");
	int check();
	~RatioNum(void);
};
