#include "my_string_stream.h"

using namespace boost::spirit::standard;
using namespace boost::spirit::qi;

my_string_stream::my_string_stream(const std::string& str) :
    it_( str.cbegin() ),
    end_ ( str.cend() ),
    fail_(false)
{
}


my_string_stream::~my_string_stream(void)
{
}

static const rule<std::string::const_iterator, std::string(), space_type> var_name = +char_("A-Z");
static const rule<std::string::const_iterator, std::string(), space_type> double_str
    = +char_("0-9") >> -( char_('.') >> +char_("0-9") );
static const rule<std::string::const_iterator, std::string(), space_type> math_operator =
    char_("+*/()-");
static const rule<std::string::const_iterator, std::string(), space_type> token =
    double_str | math_operator | var_name;

my_string_stream& operator >>(my_string_stream& input, std::string& str)
{
    str.clear();
    boost::spirit::qi::phrase_parse( input.it_, input.end_,
        token, space, str );
    if( str.empty() )
    {
        input.fail_ = true;
    }
    return input;
}

my_string_stream::operator bool()
{
    return !fail_;
}