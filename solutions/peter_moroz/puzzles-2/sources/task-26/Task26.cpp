#include "Task26.h"
#include <exception>
#include <iostream>
#include <sstream>
#include "Message.h"

using namespace std;

void Task26::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task26::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw logic_error(message.str());
  }

  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    bool success = true;
    try {
      message.CalculateDays();
    } catch (exception& ex) { 
      cerr << "Task26::Perform exception has been caught " 
          << "reason: " << ex.what() << endl;
      success = false;      
    }
    
    if (success)
      message.WriteTo(ofs_);

    message.ReadFrom(ifs_);
  }
}
void Task26::Initialize() {
  stringstream message;
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    message << "Task26::Initialize() : Can't open file "
      << input_fname() << endl;
    throw runtime_error(message.str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    message << "Task26::Initialize() : Can't open file "
      << output_fname() << endl;
    throw runtime_error(message.str());
  }
}
