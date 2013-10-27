#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <utility>
#include <string>
#include <algorithm>
using namespace std;


void findAndMarkCurrentLand(vector<string>& str, size_t nI, size_t nJ)
{
	if(str[nI].at(nJ) == '~')
		return;
	str[nI].at(nJ) = '~';

	findAndMarkCurrentLand(str, nI + 1, nJ);
	findAndMarkCurrentLand(str, nI - 1, nJ);
	findAndMarkCurrentLand(str, nI, nJ + 1);
	findAndMarkCurrentLand(str, nI, nJ - 1);
}

void readFile(ifstream& stream, vector<string>& str)
{
    string line, lineFill;
	size_t lineLength = 0, strSize = 0;
	size_t nI = 0;
    if(stream.eof())
		return;
    getline(stream, line);
	lineLength = line.length();
	for(nI = 0; nI < lineLength + 2; nI++)
		lineFill.append("~");

    str.push_back(lineFill);
    while(true)
	{
		line.append("~");
        str.push_back(string("~")+line);
		if (stream.eof())
			break;
        getline(stream, line);
    }
    str.push_back(lineFill);

}
void findLand(vector<string>& str)
{
    ofstream output_file(SOURCE_DIR "/Output.txt");
	size_t nI = 0, nJ = 0, count = 0;
	if(!output_file.is_open())
	{
		cout << "Could not open output file" << endl;
		return;
	}

    const size_t columnCount = str.size();
	const size_t rowCount = str[0].length();

    for(nI = 0; nI < columnCount; nI++)
	{
        for(nJ = 0; nJ < rowCount; nJ++)
		{
            if(str[nI][nJ] == 'o')
			{
                findAndMarkCurrentLand(str, nI, nJ);
                count++;
            }
        }
    }
    output_file << count;
    output_file.close();
}
int main()
{
	setlocale(LC_ALL, "Russian");
    ifstream input_file(SOURCE_DIR "/Input.txt");
    vector<string> str;
	if(!input_file.is_open())
	{
		cout << "Could not open input file" << endl;
		return 1;
	}
	readFile(input_file, str);
    input_file.close();
	findLand(str);
    return 0;
}

