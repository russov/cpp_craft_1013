#include "Deals.h"
#include <string>

int main(int argc, char** argv)
{
	const string searchPath = "D:\\LekS Files\\Ccraft\\cpp_craft_1013\\solutions\\aleks_apalikov\\Crafting 2\\sources\\buffer\\";
	string s =  searchPath + "2.4";
	Deals d(s);
	d.createOutput();
	return 0;
}