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
int Islands::Trace()
{
	count = 1;
	if((rows == 1) && (columns == 1))
	{
		count = Map[0][0];
		return count;
	}
	if((rows == 2) && (columns == 1))
	{
		count = ((Map[0][0] == 1) || (Map[1][0] == 1)) ? 1 : 0; 
		return count; 
	}
	if((rows == 1) && (columns == 2))
	{
		count = ((Map[0][1] == 1) || (Map[0][1] == 1)) ? 1 : 0;
		return count;
	}
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if(j < columns - 1)
			{
				int& right = Map[i][j+1];
				int& cur = Map[i][j];
				if ( cur != 0 && right != 0 )
				{
					if (cur > right)
					{
						right = cur;
					}
					else if(cur < right)
					{
						int oldCur = cur;
						cur = right;
						changeAll(oldCur, cur);
					}
					else if((cur == 1) && (right == 1))
					{
						count++;
						right = count;
						cur = count;
					}
				}
			}
			if(i < rows - 1)
			{
				int& lower = Map[i+1][j];
				int& cur1 = Map[i][j];
				if(cur1 != 0 && lower != 0)
				{
					if (cur1 > lower)
					{
						lower = cur1;
					}
					else
					{
						count++;
						lower = count;
						cur1 = count;
					}
				}
			}
		}
	}
	return count;
}

void Islands::changeAll(const int old, const int ne )
{
	if((old == 1) || (ne == 1))
		return;
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if(Map[i][j] == old)
				Map[i][j] = ne;
		}
	}

	
}

int Islands::Count()
{
	int trace = Trace();
	if (count < 2) // no groups was founded
	{
		int summ = 0;
		for(int i = 0; i < rows; ++i)
		{
			for(int j = 0; j < columns; ++j)
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
	int* numbers = new int[count];
	int k;
	for(k = 0; k < count; k++)
	{
		numbers[k] = 0;
	}
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			int cur = Map[i][j];
			if(cur == 1)
			{
				numbers[1]++;
			}
			else
			{
				numbers[cur] = 1;
			}
		}
	}
	int sum = 0;
	for(k = 0; k < count; k++)
	{
		sum += numbers[k];
	}
	of << sum;
	return sum;
}
