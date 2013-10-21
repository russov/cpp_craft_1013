#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

void adaptString(string&, bool);
void inverse(string&);

int main()
{
	ifstream input( SOURCE_DIR "/sources/task1_3/input.txt");
	ofstream output( SOURCE_DIR "/tests/task1_3/output.txt");

	if ( !input )
	{
		cerr << "Failed to open input file\n";
		return 1;
	}
	if ( !output )
	{
		cerr << "Failed to create output file\n";
		return 1;
	}

	string text;
	string key;

	bool toInvert = false;
	getline(input, text);

	adaptString(text, toInvert);

	toInvert = true;
	while( getline( input, key ) )
	{
		adaptString( key, toInvert );
		if ( text.find( key ) != string::npos)
		{
			output << "YES\n";
		}
		else
		{
			output << "NO\n";
		}
	}

	input.close();
	output.close();
	return 0;
}

void adaptString( string& target, bool toInverse )
{
	for ( int i = 0; i < target.size(); i++ )
	{
		if ( isalpha(target.at(i)) )
		{
			if ( !islower( target.at(i) ) )
			{
				target.at(i) = tolower( target.at(i) );
			}
		}
		else if (target.at(i) == ' ' || target.at(i) == '\\'
			|| target.at(i) == '-' )
		{
			target.erase( i, 1 );
			i--;
		}
	}

	if (toInverse)
	{
		reverse( target.begin(), target.end() );
	}
}
