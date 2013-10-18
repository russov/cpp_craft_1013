#include "Task26.h"
#include <iostream>
#include <sstream>
#include "Message.h"

using namespace std;

void Task26::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task26::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw Exception(message.str().c_str());
  }

  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    bool success = true;
    try {
      message.CalculateDays();
    } catch (Date::Exception& ex) { 
      cerr << "Task26::Perform - Date::Exception has been caught " 
          << "reason: " << ex.reason() << endl;
      success = false;      
    }
    
    if (success)
      message.WriteTo(ofs_);

    message.ReadFrom(ifs_);
  }
}
void Task26::Initialize() {
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    stringstream message;
    message << "Task26::Initialize() : Can't open file "
      << input_fname() << endl;
    throw Exception(message.str().c_str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    stringstream message;
    message << "Task26::Initialize() : Can't open file "
      << output_fname() << endl;
    throw Exception(message.str().c_str());
  }
}
