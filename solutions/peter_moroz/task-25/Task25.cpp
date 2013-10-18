#include "Task25.h"
#include <sstream>

using namespace std;

void Task25::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task25::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw Exception(message.str().c_str());
  }


  Message message;
  message.ReadFrom(ifs_);

  unsigned start_time = message.time();
  unsigned current_time = start_time;

  statistics_.Reset();
  statistics_.set_start_time(start_time);

  messages_buffer_.Clear();

  while(!ifs_.eof()) {
    if (message.type() <= kMaximalAllowedMessageType) {
      if(message.time() > current_time) {
        messages_buffer_.Flush();
        current_time = message.time();
      }

      messages_buffer_.PutMessage(message);
      message.ReadFrom(ifs_);
    } // if (message.type() <= kMaximalAllowedMessageType)
  } // while(!ifs_.eof())

  statistics_.DumpOnTime(ofs_, message.time());
}
void Task25::Initialize() {
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    stringstream message;
    message << "Task25::Initialize() : Can't open file "
      << input_fname() << endl;
    throw Exception(message.str().c_str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    stringstream message;
    message << "Task25::Initialize() : Can't open file "
      << output_fname() << endl;
    throw Exception(message.str().c_str());
  }
}
