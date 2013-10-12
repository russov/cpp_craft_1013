#ifndef EARTH_SURFACE_H_
#define EARTH_SURFACE_H_

#include <fstream>
#include <string>
#include <vector>

#include "Point.h"

class EarthSurface {

public:
  class Exception {
  public:
    Exception(const char* reason) 
      : reason_(reason) {}

    const std::string& reason() const {
      return reason_;
    }
  private:
    std::string reason_;
  };

public:
  EarthSurface() {}
  ~EarthSurface() {}

  void ReadFromStream(std::ifstream& ifs);

  int GetRowsCount() const;
  int GetColsCount() const;

  bool IsLand(const Point& pt) const;

private:
  std::vector<std::string> layout_;
}; // EarthSurface

#include "EarthSurface-inl.h"

#endif // EARTH_SURFACE_H_
