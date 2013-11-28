#pragma once
#include <fstream>
#include <vector>

typedef int code_t;
using namespace std;
class RatioNum
{
	ifstream fs;
	ofstream of;
	int count;
	vector<code_t> codes;
public:
	RatioNum(const string& fileName = "Input2.txt");
	static void trunc(double& num);
	void push_code(double num);
	static code_t convert(double);
	int check();
	~RatioNum(void);
};
