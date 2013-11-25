#include "buf.h"
#include <string>

int main(int argc, char** argv)
{
	std::string s = BINARY_DIR "2.5";
	buf b(s);
	b.createOutput();
	return 0;
}