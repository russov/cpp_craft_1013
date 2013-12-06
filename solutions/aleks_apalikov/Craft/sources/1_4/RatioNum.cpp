#include "RatioNum.h"
#include <math.h>
#include <string>

RatioNum::RatioNum(const string& fileName)
{
	string str = BINARY_DIR + string("/") + fileName;
	fs.open(str.c_str(), fstream::in);
	
	int n = 0;
	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	fs >> n;
	double cod;
	for (int i = 0; i < n; i++)
	{
		fs >> cod;
		push_code(cod);
	}
	string out_s = BINARY_DIR + string("/") + "Output2.txt";
	of.open(out_s.c_str(), fstream::out  | fstream::trunc );

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
	double pass;
	bool concur;
	while (!fs.eof())
	{
		concur = false;
		char c[20];
		if(fs.peek() == EOF)
			break;
		fs>>pass;
		code_t pass_i = convert(pass);
		for(vector<code_t>::iterator it = codes.begin(); it < codes.end(); it++)
		{
			concur |= (*it == pass_i);
		}
		string ans = concur ? "Yes\n" : "No\n";
		of << ans.c_str();
		fs.getline(c, 20); //read all spaces after float and \n symbol
	}
	
	return count;
}

void RatioNum::trunc( double& num )
{
	num = floor(num * 10000) / 10000;

}


void RatioNum::push_code( double num)
{
	codes.push_back(convert(num));
}

code_t RatioNum::convert( double d)
{
	return (code_t) (d * 10000);
}
