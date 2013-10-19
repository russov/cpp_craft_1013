#ifndef DATE_H_
#define DATE_H_

#include <string>

class Date {

  static const unsigned kDaysInMonth = 31;
  static const unsigned kMonthsInYear = 12;
  static const unsigned kDaysInYear = kDaysInMonth * kMonthsInYear;

public:
  class Exception {
  public:
    Exception(const std::string& reason) : reason_(reason) {}
    const std::string& reason() const { return reason_; }
  private:
    std::string reason_;
  };

private:
  Date() : d_(0), m_(0), y_(0) {}

public:
  explicit Date(const char* s);

  static Date ParseString(const char* s);

private:
  static bool CheckDayValue(unsigned dv) {
    return (dv > 0 && dv <= kDaysInMonth);
  }
  static bool CheckMonthValue(unsigned mv) {
    return (mv > 0 && mv <= kMonthsInYear);
  }
  static bool CheckYearValue(unsigned yv) {
    return (yv > 0);
  }

public:
  unsigned GetDaysSinceChristmas() const {
    return ((y_ - 1) * kDaysInYear + (m_ - 1) * kDaysInMonth + d_);
  }

private:
  unsigned d_;
  unsigned m_;
  unsigned y_;
};

#endif // DATE_H_
