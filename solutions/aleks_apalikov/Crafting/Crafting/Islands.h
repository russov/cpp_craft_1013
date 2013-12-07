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
	int Trace();
public:
	Islands(void);
	int Count();
	void changeAll(const int old, const int ne);
	~Islands(void);
};
