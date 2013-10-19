#include "Task24.h"
#include "Message.h"
#include <sstream>

using namespace std;

bool Task24::IsAllowedMessageType(unsigned type) {
  return (type == Message::MT_MarketOpen ||
          type == Message::MT_Trade || 
          type == Message::MT_Quote || 
          type == Message::MT_MarketClose);
}

void Task24::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task24::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw Exception(message.str().c_str());
  }

  unsigned max_time = 0;
  unsigned allowed_time = 0;
  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    // filter messages here
    if (IsAllowedMessageType(message.type())) {
      if (message.time() > max_time) {
        max_time = message.time();
        allowed_time = max_time - 2;
        message.WriteTo(ofs_);
      } else {
        if (message.time() > allowed_time)
          message.WriteTo(ofs_);
      }
    } // if (IsAllowedMessageType(message.type()))
    message.ReadFrom(ifs_);
  }

}
void Task24::Initialize() {
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    stringstream message;
    message << "Task24::Initialize() : Can't open file "
      << input_fname() << endl;
    throw Exception(message.str().c_str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    stringstream message;
    message << "Task24::Initialize() : Can't open file "
      << output_fname() << endl;
    throw Exception(message.str().c_str());
  }
}
