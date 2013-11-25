#include "Deals.h"
#include <string>

int main(int argc, char** argv)
{
	const string searchPath = BINARY_DIR + string("/");
	string s =  searchPath + "input_242.txt";
	string out_f = searchPath + "output_242.txt";
	Deals d(s, out_f);
	d.createOutput();
	return 0;
}