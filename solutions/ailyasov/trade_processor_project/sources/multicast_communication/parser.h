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

    template <typename T> class converter
    {
        public:
            static void convert_block(message_type const& block, std::vector< boost::shared_ptr<T> >& v)
            {
                std::vector<message_type> m;
                split_block(block, m);
                convert(m, v);
            }
        private:
            static void convert(
                    std::vector<message_type> const& m , std::vector< boost::shared_ptr<T> >& v)
            {
                for(std::vector<message_type>::const_iterator it = m.begin(); it != m.end(); ++it)
                {
                    message_type m = *it;
                    try
                    {
                        if( !m.empty() && T::is_valid_type(m) )
                        {
                            boost::shared_ptr<T> t( new T( m ) );
                            v.push_back( t );
                        } else
                        {
                            std::cerr << "incorrect message" << std::endl;
                        }
                    } catch(std::logic_error& e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            } 


            static void split_block(message_type const& m, std::vector<message_type>& v)
            {
                message_type msg;
                for(message_type::const_iterator it = m.begin(); it != m.end(); ++it)
                {
                    char c = *it;
                    switch(c)
                    {
                        case 0x01:
                            msg.clear();
                            break;
                        case 0x1f:
                            v.push_back(msg);
                            msg.clear();
                            break;
                        case 0x03:
                            v.push_back(msg);
                            return;
                        default:
                            msg.push_back(c);
                    }
                }
            }

    };

}

#endif
