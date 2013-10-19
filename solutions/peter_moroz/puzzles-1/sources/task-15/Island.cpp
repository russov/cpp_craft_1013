#include "Island.h"

Island::Island() {}
Island::~Island() {}

Island::Island(const Point& seed) {
  parts_.push_back(seed);
}
