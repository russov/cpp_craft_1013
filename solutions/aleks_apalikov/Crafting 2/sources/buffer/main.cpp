#include "Deals.h"
#include "buf.h"
#include "Datafeed.h"
#include <string>
#define BINARY_DIR "D:\\LekS Files\\Ccraft\\cpp_craft_1013\\solutions\\aleks_apalikov\\Crafting 2\\sources\\buffer\\"

int main(int argc, char** argv)
{
	string searchPath = BINARY_DIR;
	string s =  searchPath + "2.4";
	Deals d(s);
	d.createOutput();
	s = searchPath + "2.5";
	buf b(s);
	b.createOutput();
	s = searchPath + "2.6";
	Datafeed df(s);
	df.createOutput();
	return 0;
}