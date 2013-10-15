#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int calculateIslands( const vector< string > );
void exploreIsland( int, int, const vector< string >&, bool** );

int main() 
{	
	vector<string> map;
	string buffer;
		
	ofstream outFile;
	ifstream mapSource( SOURCE_DIR "/sources/task1_5/input.txt" );
	
	if ( !mapSource ) 
	{
		cerr << "Failed to open input file\n";
		return 1;
	}

	while ( getline(mapSource, buffer) )
	{
		map.push_back( buffer );
	}

	mapSource.close();
	
	int numberOfIslands = calculateIslands(map);

	outFile.open( SOURCE_DIR "/tests/task1_5/output.txt" );
	
	if ( !outFile )
	{
		cerr << "Output File was not created\n";
		cout << "Number of islands is " << numberOfIslands << endl;
		return 1;
	}

	outFile << numberOfIslands << endl;

	outFile.close();

	return 0;
}

int calculateIslands( const vector< string > map)
{
	int islandsNumber = 0;
	int lineSize;
	if ( map.size() > 0 )
	{
		lineSize = map.at(0).length();		
	} 
	else 
	{
		return 0;
	}

	bool **isPartVisited = new bool*[map.size()];
	for ( int i = 0; i < map.size(); i++ )
	{		
		isPartVisited[i] = new bool[lineSize];
		for ( int j = 0; j < lineSize; j++ )
		{
			isPartVisited[i][j] = false;
		}
	}

	
	// find coast and explore an island
	for ( int i = 0; i < map.size(); i++ )
	{		
		for ( int j = 0; j < lineSize; j++ )
		{
			char part = map.at(i).at(j);
			if ( part == 'o' && !isPartVisited[i][j] )
			{
				islandsNumber++;
				exploreIsland(i, j, map, isPartVisited);
			}
		}
	}

	for ( int i = 0; i < map.size(); i++ )
	{
		delete [] isPartVisited[i];
	}
	delete [] isPartVisited;
	
	return islandsNumber;
}
void exploreIsland( int coordX, int coordY, const vector< string >& map, bool** isVisited)
{
	isVisited[coordX][coordY] = true;

	if ( coordX - 1 >= 0 && map.at(coordX - 1).at(coordY) == 'o'
		&& !isVisited[coordX - 1][coordY] )
	{
		exploreIsland( coordX - 1, coordY, map, isVisited );
	}

	if ( coordY + 1 < map.at(0).length()  && map.at(coordX).at(coordY + 1) == 'o'
		&& !isVisited[coordX][coordY + 1] )
	{
		exploreIsland( coordX, coordY + 1, map, isVisited );
	}

	if ( coordX + 1 < map.size()  && map.at(coordX + 1).at(coordY) == 'o'
		&& !isVisited[coordX + 1][coordY] )
	{
		exploreIsland( coordX + 1, coordY, map, isVisited);
	}

	if ( coordY - 1 >= 0 && map.at(coordX).at(coordY - 1) == 'o'
		&& !isVisited[coordX][coordY - 1] )
	{
			exploreIsland( coordX, coordY - 1, map, isVisited);
	}
}