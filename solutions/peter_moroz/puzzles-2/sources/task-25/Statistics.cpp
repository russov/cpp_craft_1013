#include "Statistics.h"
#include <iostream>

using namespace std;

void Statistics::DumpOnTime(ostream& os, unsigned t) {
  // TO DO: check if (t > start_time_)
  unsigned duration = t - start_time_ + 1;
  map<MessageType, unsigned>::iterator it = counters_.begin();
  while (it != counters_.end()) {
    unsigned type = it->first;
    unsigned count = it->second;
    double avg = count;
    count /= duration;

    os.write(reinterpret_cast<const char*>(&type), sizeof(type));
    os.write(reinterpret_cast<const char*>(&avg), sizeof(avg));

    ++it;
  } // while (it != counters_.end())
}
