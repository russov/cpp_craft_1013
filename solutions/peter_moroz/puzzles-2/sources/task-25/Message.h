#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

typedef unsigned MessageType;

class Message {

public:
  Message() : type_(0), time_(0) {}

  void ReadFrom(std::ifstream& ifs);
  size_t GetTotalSize() const {
    return (sizeof(type_) + sizeof(time_) + text_.size());
  }

  MessageType type() const { return type_; }
  unsigned time() const { return time_; }

private:
  MessageType type_;
  unsigned time_;
  std::string text_;
};

#endif // MESSAGE_H_
