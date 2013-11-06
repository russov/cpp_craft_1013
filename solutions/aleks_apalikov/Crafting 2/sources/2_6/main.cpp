#include "2_6/Datafeed.h"
#include <string>
#define BINARY_DIR "D:\\LekS Files\\Ccraft\\cpp_craft_1013\\solutions\\aleks_apalikov\\Crafting 2\\sources\\buffer\\"

int main(int argc, char** argv)
{
	std::string s = BINARY_DIR "input.txt";
	Datafeed df(s);
	df.createOutput();
	return 0;
}