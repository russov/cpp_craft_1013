#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

void initArray( vector< double >&, ifstream& );
void processPasswords( vector< double >&, ifstream& );
bool myBinarySearch( vector< double >, int, int, double );
bool find_equal( vector< double >, double );

int main()
{
	const int n = 30;
	int b[n];
	vector<double> codes;	
	ifstream inputFile( SOURCE_DIR "/sources/task1_4/input.txt" );

	if ( !inputFile ) 
	{
		cerr << "Failed to open input file" << endl;
		return 1;
	}	

	initArray( codes, inputFile );
	processPasswords( codes, inputFile);

	inputFile.close();
	return 0;
}

void initArray( vector< double >& codes, ifstream& inputFile ) 
{
	int N;
	double buffer;

	inputFile >> N;	

	for (int i = 0; i < N; i++) 
	{
		inputFile >> buffer;
		codes.push_back(buffer);	
	}

	sort(codes.begin(), codes.end());
}

void processPasswords( vector<double>& codes, ifstream& inputFile ) 
{
	ofstream outFile( SOURCE_DIR "/tests/task1_4/output.txt" );

	if ( !outFile ) 
	{
		cerr << "Failed to create output file" << endl;
		exit( 1 );
	}

	double nextPassword;

	while ( inputFile >> nextPassword )
	{
		if ( find_equal(codes, nextPassword) )
		{
			outFile << "YES" << endl;
		}
		else
		{
			outFile << "NO" << endl;
		}
	}

	outFile.close();
}

bool find_equal( vector< double > codes, double password) 
{
	return myBinarySearch( codes, 0, codes.size(), password );
}

bool myBinarySearch( vector< double > codes, int left,
					int right, double element )
{
	double eps = 0.0001;
	
	if ( left >= right ) 
	{
		return false;
	}	

	int middle = left + ( right - left ) / 2;
	
	if ( left == right - 1 ) 
	{
		double difference = fabs( element ) - fabs( codes.at(left) );
		return (  difference < eps && difference > 0);
	}
	
	if( element < 0 && element <= codes.at(middle)
		&& ( codes.at(middle) - element ) < eps )
	{
		return true;
	}
	else if ( codes.at(middle) <= element 
		&& ( element - codes.at(middle) ) < eps )
	{
		return true;		
	} 
	else if ( codes.at(middle) < element )
	{
		return myBinarySearch( codes, middle + 1, right, element );		
	}
	else 
	{
		 return myBinarySearch( codes, left, middle, element );
	}
}