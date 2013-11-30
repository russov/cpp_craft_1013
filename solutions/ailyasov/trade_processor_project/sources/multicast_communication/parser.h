#ifndef _MULTICAST_COMMUNICATION_PARSER_H_
#define _MULTICAST_COMMUNICATION_PARSER_H_

#include <vector>
#include <string>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

namespace multicast_communication
{
    typedef std::vector<char> message_type;

    template <typename T>
        class parser : private virtual boost::noncopyable
    {
        public:
            parser(message_type const& m) : raw_(m)
        {
        } 
            message_type extract_field(int f)
            {
                return message_type(raw_.begin() + T::OFFSET[f], raw_.begin() + T::OFFSET[f] + T::LENGTH[f]);
            }

            template <typename N>
                N extract(int f)
                {
                    message_type m = extract_field(f);
                    return boost::lexical_cast<N>( std::string(m.begin(), m.begin() + m.size()) );
                } 


            static bool is_valid_type(message_type const& m)
            {
                for(int i = 0; i < T::RESTRICTION_SIZE; ++i)
                {
                    if( m[0] == T::RESTRICTION_CATEGORY[i] && m[1] == T::RESTRICTION_TYPE[i] )
                        return true;
                }
                return false;
            }

        private:
            message_type raw_;
    };
}

#endif
