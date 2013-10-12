#ifndef ISLAND_INL_H_
#define ISLAND_INL_H_

#include <algorithm>

inline bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.col_ == rhs.col_ && lhs.row_ == rhs.row_);
}

inline void Island::AddPart(const Point& p) {
  parts_.push_back(p);
}
inline bool Island::Contains(const Point& p) const {
  return (find(parts_.begin(), parts_.end(), p) != parts_.end());
}

#endif // ISLAND_INL_H_