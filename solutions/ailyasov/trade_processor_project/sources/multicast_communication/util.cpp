#include <stdexcept>
#include <iostream>
#include "util.h"

namespace multicast_communication
{ 
    const util::denom_type::value_type raw_data[] = { 
        util::denom_type::value_type('0', 1),
        util::denom_type::value_type('3', 8),
        util::denom_type::value_type('4', 16),
        util::denom_type::value_type('5', 32),
        util::denom_type::value_type('6', 64),
        util::denom_type::value_type('7', 128),
        util::denom_type::value_type('8', 256),
        util::denom_type::value_type('A', 10),
        util::denom_type::value_type('B', 100),
        util::denom_type::value_type('C', 1000),
        util::denom_type::value_type('D', 10000),
        util::denom_type::value_type('E', 100000),
        util::denom_type::value_type('F', 1000000),
        util::denom_type::value_type('G', 10000000),
        util::denom_type::value_type('H', 100000000),
        util::denom_type::value_type('I', 1)
    };

    const int num_elems = sizeof raw_data / sizeof raw_data[0];
    util::denom_type util::denom_map(raw_data, raw_data + num_elems);


    int util::get_denom(char c)
    {
        denom_type::iterator it = denom_map.find(c);
        if(it == util::denom_map.end())
        {
            std::cerr << c << " wrong denominator code." << std::endl;
            throw std::logic_error("wrong denominator code.");
        }
        int denom = it->second;
        if(denom == 0)
        {
            std::cerr << "Zero denominator." << std::endl;
            throw std::logic_error("Zero denominator");
        }
        return denom;
    }
}
