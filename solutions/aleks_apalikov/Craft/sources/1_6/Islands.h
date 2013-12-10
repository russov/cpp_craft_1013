#pragma once
#include <fstream>
#include <vector>

using namespace std;
void make_water(int pos_x, int pos_y, vector<vector<int>>& map); // all lower points
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
	Islands(const string& f = "Input3.txt");
	int Count();
	void changePrev(int old, int ne, size_t pos_x, size_t pos_y);
	void changeLowLeft(int old, int ne,  size_t pos_x, size_t pos_y);
	~Islands(void);
};
