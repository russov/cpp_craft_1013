#include "reversedStr.h"
#include <string>
#include <algorithm>

using namespace std;
const string ReversedStr::path = "F:\\Cpp_Craft\\cpp_craft_1013\\solutions\\aleks_apalikov\\Crafting\\Crafting\\";


ReversedStr::ReversedStr(string fileName)
{
	string str = path + fileName;
	fs.open(fileName.c_str(), fstream::in);

	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	getline(fs, input);
	of.open("Output.txt", fstream::out  | fstream::trunc );

	//str = path + "Output.txt";
	if(!of.is_open()) {
		printf("Output failed to open!");
	}

	lowerInput = toLower(input);


}

ReversedStr::~ReversedStr(void)
{
	fs.close();
	of.close();
}

int ReversedStr::findNext()
{
	getline(fs, current);
	if(current.length() <= 1)
	{
		return -1;
	}
	string rev = "";
	for( string::reverse_iterator it = current.rbegin(); it < current.rend(); it++)
	{
		rev += *it;
	}
	rev = toLower(rev);
	if(lowerInput.find(rev, 0) != string::npos)
	{
		of << "Yes\n";
		count ++;
	}
	else
	{
		of << "No\n";
	}
	return count;
}

int ReversedStr::findAll()
{
	int i = 0;
	while((i = findNext()) != -1)
	{
		cout<<"Founded "<< i << endl;
	}
	return i;
}

string ReversedStr::toLower(string in)
{
	string low;
	for( string::iterator it = in.begin(); it < in.end(); it++)
	{
		if(*it != ' ' && *it != '\\' && *it != '-')
		{
			low += *it;
		}
	}
	std::transform(low.begin(), low.end(), low.begin(), ::tolower);
	return low;
}

