#pragma once
#include <fstream>
#include <vector>

using namespace std;
void make_water(int pos_x, int pos_y, vector<vector<int>>& map); // all lower points
class Islands
{
	ifstream fs;
	ofstream of;
	size_t rows;
	size_t columns;
	vector<vector<int>> Map;
	void Trace();
public:
	Islands(const string& f = "Input3.txt");
	int Count();
	~Islands(void);
};
