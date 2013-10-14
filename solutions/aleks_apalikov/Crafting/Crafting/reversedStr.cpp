#include "reversedStr.h"
#include <string>

using namespace std;
const string reversedStr::path = "F:\\Cpp_Craft\\cpp_craft_1013\\solutions\\aleks_apalikov\\Crafting\\Crafting\\";


reversedStr::reversedStr(string fileName)
{
	string str = path + fileName;
	fs.open(fileName.c_str(), fstream::in);

	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	getline(fs, input);
	of.open("Output.txt", fstream::out  | fstream::trunc );

	if(!of.is_open()) {
		printf("Output failed to open!");
	}
	str = path + "Output.txt";

}

reversedStr::~reversedStr(void)
{
	fs.close();
	of.close();
}

int reversedStr::findNext()
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
	if(input.find(rev, 0) == string::npos)
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

int reversedStr::findAll()
{
	int i = 0;
	while((i = findNext()) != -1)
	{
		cout<<"Founded "<< i << endl;
	}
	return i;
}

