#include "Islands.h"
#include <assert.h>
#include <string>

Islands::Islands(const string& fileName)
{
	string str = BINARY_DIR + string("/") + fileName;
	fs.open(str.c_str(), fstream::in);

	int n = 0;
	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	char cod;
	rows = 1;
	columns = 0;
	int cols = 0;
	string current;
	while (!fs.eof())
	{
		Map.resize(rows);
		getline(fs, current);
		for(string::const_iterator it = current.begin(); it < current.end(); it++)
		{
			cod = *it;
			if(cod == 'o') Map[rows - 1].push_back(1);
			if(cod == '~') Map[rows - 1].push_back(0);
		}
		rows ++;
	}
	if (Map[rows-2].size() == 0) //del empty line
	{
		rows-=2;
		Map.pop_back();
	}
	else
	{
		rows --;
	}
	if(Map.size() == 0) //empty file
	{
		Map.resize(1);
		Map[0].push_back(0);
		rows = 1;
	}
	columns = Map[0].size();
	string out_s = BINARY_DIR + string("/") + "Output3.txt";
	of.open(out_s.c_str(), fstream::out  | fstream::trunc );

	if(!of.is_open()) {
		printf("Output3 failed to open!");
	}
}

Islands::~Islands(void)
{
	of.close();
	for(vector<vector<int>>::iterator it = Map.begin(); it < Map.end(); it++)
	{
		it->erase(it->begin(), it->end());
	}

	Map.erase(Map.begin(), Map.end());
}
enum {
	forbidden = 4
};
void make_water( size_t pos_x, size_t pos_y, vector<vector<int>>& map)
{
	size_t x = pos_x;
	size_t y = pos_y;
	if(pos_x < map.size() - 1)
	{
		if( map[ ++ x][y] == 1)
		{
			int & low = map[ x][y];
			low = 0;
			make_water(x, y, map);
		}
	}
	if(pos_y < map[0].size() - 1)
	{
		if ((map[ pos_x][++y] & 0x3) != 0)
		{
			int& right = map[pos_x][y];
			right = 0;
			make_water(pos_x, y, map);
		}
	}
	if(pos_y > 1)
	{
		y = pos_y - 1;
		if( (map[ pos_x][y] & 0x3) != 0 )
		{
			int & left = map[pos_x][y];
			if(left != forbidden)
			{
				left = 0;
				make_water(pos_x, y, map);
			}
		}
	}
	return;
	
}

void Islands::Trace()
{
	for(size_t i = 0; i < rows; ++i)
	{
		for(size_t j = 0; j < columns; ++j)
		{
			if(j < columns - 1)
			{
				int& right = Map[i][j+1];
				int& cur = Map[i][j];
				if ( cur != 0 && right != 0 )
				{
					cur = forbidden;
					make_water(i, j, Map);
					cur = 1;
				}
			}
			if(i < rows - 1)
			{
				int& lower = Map[i+1][j];
				int& cur1 = Map[i][j];
				if(cur1 != 0 && lower != 0)
				{
					cur1 = forbidden;
					make_water(i, j, Map);
					cur1 = 1;
				}
			}
		}
	}
	return;
}

int Islands::Count()
{
	if(rows == 0 && columns == 0)
		return 0;
	if(rows == 1 && columns == 1)
		return Map[0][0];
	Trace();
	int summ = 0;
	for(size_t i = 0; i < rows; ++i)
	{
		for(size_t j = 0; j < columns; ++j)
		{
			if( Map[i][j] == 1)
			{
				summ++;
			}
		}
	}
	of << summ;
	return summ;
}