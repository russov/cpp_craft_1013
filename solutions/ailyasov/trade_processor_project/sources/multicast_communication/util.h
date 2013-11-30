#ifndef _MULTICAST_COMMUNICATION_UTIL_H_
#define _MULTICAST_COMMUNICATION_UTIL_H_

#include <map>

namespace multicast_communication
{
    struct util
    {
        typedef std::map<char, int> denom_type;
        static denom_type denom_map;

        static int get_denom(char);

    };
}

#endif
