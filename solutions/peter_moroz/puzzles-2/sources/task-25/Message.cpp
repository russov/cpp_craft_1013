#include "Message.h"

#include <fstream>

using namespace std;

void Message::ReadFrom(ifstream& ifs) {
  ifs.read(reinterpret_cast<char*>(&type_), sizeof(type_));
  ifs.read(reinterpret_cast<char*>(&time_), sizeof(time_));
  unsigned text_length = 0;
  ifs.read(reinterpret_cast<char*>(&text_length), sizeof(text_length));
  text_.resize(text_length);
  for (unsigned i = 0; i < text_length; ++i) {
    char x = ' ';
    ifs.read(&x, sizeof(x));
    text_[i] = x;
  }
}
