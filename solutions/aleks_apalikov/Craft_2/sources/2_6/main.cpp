#include "2_6/Datafeed.h"
#include <string>

int main(int argc, char** argv)
{
	std::string s = BINARY_DIR "/input_261.txt";
	Datafeed df(s);
	df.createOutput();
	return 0;
}