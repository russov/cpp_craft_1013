#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

typedef unsigned MessageType;

class Message {
public:
  enum Types {
    MT_MarketOpen = 1,
    MT_Trade,
    MT_Quote,
    MT_MarketClose
  };

public:
  Message() : type_(0), time_(0) {}

  MessageType type() const { return type_; }
  unsigned time() const { return time_; }

  size_t GetTotalSize() const {
    return (sizeof(type_) + sizeof(time_) + text_.size());
  }
  void ReadFrom(std::istream& is);
  void WriteTo(std::ostream& os) const;

private:
  MessageType type_;
  unsigned time_;
  std::string text_;
};

#endif // MESSAGE_H_
