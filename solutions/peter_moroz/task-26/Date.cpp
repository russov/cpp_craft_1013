#include <assert.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include "Date.h"

using namespace std;

Date::Date(const char* s) {
  (*this) = ParseString(s);
}
Date Date::ParseString(const char* s) {
  assert(s != NULL);
  stringstream msg;
  char buff[5];
  Date d;

  if (::strlen(s) < 8) {
    msg << "Date::ParseString - invalid date string: "
      << s << ", it must has format YYYYMMDD" << endl;
    throw Exception(msg.str());
  }

  ::memset(buff, 0, sizeof(buff));
  ::strncpy_s(buff, sizeof(buff), s, 4);
  d.y_ = ::atoi(buff);
  if (!CheckYearValue(d.y_)) {
    msg << "Date::ParseString - invalid year value, "
        << "it must be greater than 0" << endl;
    throw Exception(msg.str());
  }

  ::memset(buff, 0, sizeof(buff));
  ::strncpy_s(buff, sizeof(buff), s + 4, 2);
  d.m_ = ::atoi(buff);
  if (!CheckMonthValue(d.m_)) {
    msg << "Date::ParseString - invalid month value, "
        << "it must be in range [1; 12] " << endl;
    throw Exception(msg.str());
  }

  ::memset(buff, 0, sizeof(buff));
  ::strncpy_s(buff, sizeof(buff), s + 6, 2);
  d.d_ = ::atoi(buff);
  if (!CheckMonthValue(d.m_)) {
    msg << "Date::ParseString - invalid month value, "
      	<< "it must be in range [1; 12] " << endl;
    throw Exception(msg.str());
  }
  
  return d;
}
