#pragma once

#include <boost/spirit/include/qi.hpp>


class my_string_stream 
{
    std::string::const_iterator it_;
    std::string::const_iterator end_;

    bool fail_;

    friend my_string_stream& operator >>(my_string_stream& input, std::string& str);
public:
    explicit my_string_stream(const std::string& str);
    ~my_string_stream(void);
    operator bool();
};

my_string_stream& operator >>(my_string_stream& input, std::string& str);
