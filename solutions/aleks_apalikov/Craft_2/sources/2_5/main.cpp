#include "buf.h"
#include <string>

int main(int argc, char** argv)
{
	std::string s = BINARY_DIR "/input_251.txt";
	buf b(s);
	b.createOutput();
	return 0;
}