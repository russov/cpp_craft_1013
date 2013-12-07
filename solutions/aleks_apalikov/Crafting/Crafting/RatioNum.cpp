#include "RatioNum.h"
#include <math.h>

RatioNum::RatioNum(string fileName)
{
//	string str = reversedStr::path + fileName;
	fs.open(fileName.c_str(), fstream::in);
	
	int n = 0;
	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	fs >> n;
	float cod;
	for (int i = 0; i < n; i++)
	{
		fs >> cod;
		codes.push_back(cod);
	}
	of.open("Output2.txt", fstream::out  | fstream::trunc );

	//str = path + "Output.txt";
	if(!of.is_open()) {
		printf("Output2 failed to open!");
	}
}

RatioNum::~RatioNum(void)
{
	fs.close();
	of.close();
}

int RatioNum::check()
{
	float pass;

	bool concur;
	while (!fs.eof())
	{
		concur = false;
		fs>>pass;
		passes.push_back(pass);
		for(vector<float>::iterator it = codes.begin(); it < codes.end(); it++)
		{
			concur |= (fabs(*it - pass) < 1e-4);
		}
		string ans = concur ? "Yes\n" : "No\n";
		of << ans.c_str();
	}
	
	return count;
}
