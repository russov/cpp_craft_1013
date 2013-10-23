#include "EarthSurface.h"

#include <exception>
#include <sstream>

using namespace std;

void EarthSurface::ReadFromStream(ifstream& ifs) {
  layout_.clear();

  string line;
  getline(ifs, line);

  size_t column_count = line.length();
  if (column_count == 0)
    throw logic_error("Zero length of top row, check input data.");

  layout_.push_back(line);

  int row_count = 0;
  while (!ifs.eof()) {
    getline(ifs, line);
    ++row_count;
    if (line.length() != column_count) {
      stringstream message;
      message << "Invalid data at row: " << row_count 
		  << ", all rows must have equal length. " << endl;
      throw logic_error(message.str());
    }
    layout_.push_back(line);
  }
}
