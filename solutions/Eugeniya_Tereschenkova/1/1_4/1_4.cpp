#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <algorithm>
#include <vector>
using namespace std;

void get_Codes(ifstream& stream, vector<long long int>& vectCodes)
{
	int nCodesCount = 0, nI = 0;
	double Passw;
	static const int nPrecisionNumbers = 10000;
	static const int nMaxCodesCount = 1000000;

	stream >> nCodesCount;
	if(stream.eof())
		return;

	if(nCodesCount <= 0 || nCodesCount > nMaxCodesCount)
		return;
	else
		vectCodes.reserve(nCodesCount);

	for( nI = 0; nI < nCodesCount && !stream.eof(); nI++) 
	{
		stream >> Passw;
		vectCodes.push_back((long long int)(Passw*nPrecisionNumbers));
	}
	sort(vectCodes.begin(), vectCodes.end());
}

bool compare_CodesAndPasswords(ifstream& stream, const vector<long long int>& vectCodes) 
{
	ofstream output_file(SOURCE_DIR "/Output.txt" );
	static const int nPrecisionNumbers = 10000;
	double Passw;
	if(!output_file.is_open())
	{
		cout << "Could not open output file" << endl;
		return false;
	}
	while(!stream.eof())
	{
		stream >> Passw;     
		
		if(binary_search(vectCodes.begin(), vectCodes.end(), (long long int)(Passw*nPrecisionNumbers)))
			output_file << "YES" << endl;
		else 
			output_file << "NO" << endl;
	}
	output_file.close();
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream input_file(SOURCE_DIR "/Input.txt" );
	vector<long long int> vectCodes;

	if(!input_file.is_open())
	{
		cout << "Could not open input file" << endl;
		return 1;
	} 
	get_Codes(input_file, vectCodes);

	if(!compare_CodesAndPasswords(input_file, vectCodes))
		return 1;
	input_file.close();
	
	return 0;
}
