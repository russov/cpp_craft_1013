#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <algorithm>
using namespace std;

bool formatting_line(ifstream& stream, string& line)
{
	static const size_t maxInputLen = 100000;
	getline(stream, line);
	if(line.empty())
		return false;
	if(line.size() > maxInputLen)
	{
		cout << "Incorrect string" << endl;
		return false;
	}
	line.erase(remove_if(line.begin(), line.end(), [](const char symb){ return (symb == ' ') || (symb == '\\') || (symb == '-'); }), line.end());
	transform(line.begin(), line.end(), line.begin(), tolower);
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream input_file(SOURCE_DIR "/Input.txt" );
	ofstream output_file(SOURCE_DIR "/Output.txt" );
	string line, key;
	if(!input_file.is_open())
	{
		cout << "Could not open input file" << endl;
		return 1;
	}
	if(!output_file.is_open())
	{
		cout << "Could not open output file" << endl;
		input_file.close();
		return 1;
	}
	if(!input_file.eof())
		if(!formatting_line(input_file, line))
		{
			input_file.close();
			output_file.close();
			return 1;
		}

	while(!input_file.eof())
	{
		formatting_line(input_file, key);
		if(key.empty())
			continue;
		reverse(key.begin(), key.end());
		if (line.find(key) != string::npos)
			output_file << "YES" << endl;
		else
			output_file << "NO" << endl;
	}

	input_file.close();
	output_file.close();
	
	return 0;
}
