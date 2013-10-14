#pragma once
#include <fstream>
#include <vector>

using namespace std;
class Islands
{
	ifstream fs;
	ofstream of;
	int count;
	int rows;
	int columns;
	vector<vector<int>> Map;
public:
	Islands(void);
	int Count();
	~Islands(void);
};
