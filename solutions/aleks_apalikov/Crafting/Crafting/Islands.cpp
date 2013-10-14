#include "Islands.h"
#include <assert.h>
#include <string>

Islands::Islands(void)
{
	//	string str = reversedStr::path + fileName;
	fs.open("input3.txt", fstream::in);

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
		rows--;
		Map.pop_back();
	}
//	cols = Map[rows-1].size();
	of.open("Output3.txt", fstream::out  | fstream::trunc );

	//str = path + "Output.txt";
	if(!of.is_open()) {
		printf("Output3 failed to open!");
	}
}

Islands::~Islands(void)
{
}

int Islands::Count()
{
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if (Map[i][j] != 0 && Map[i+1][j] != 0 && Map[i][j+1] != 0)
			{
				count++;
				if (Map[i][j])
				{
					Map[i][j] = 2;
				}
			}
		}
	}
	return count;
}
