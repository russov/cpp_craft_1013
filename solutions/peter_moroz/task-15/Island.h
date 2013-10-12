#ifndef ISLAND_H_
#define ISLAND_H_

#include <vector>
#include "Point.h"

class Island {
public:
  Island();
  ~Island();

  Island(const Point& seed);

  void AddPart(const Point& p);
  bool Contains(const Point& p) const;

private:
  std::vector<Point> parts_;
}; // Island

#include "Island-inl.h"

#endif // ISLAND_H_
