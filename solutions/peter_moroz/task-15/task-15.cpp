#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include "Island.h"
#include "EarthSurface.h"

using namespace std;

static const char* InputFilename = "Input.txt";
static const char* OutputFilename = "Output.txt";

int CountIslands(const EarthSurface& earth_surface);
vector<Point> GetNeighborhood(const Point& pt, const EarthSurface& earth_surface);
vector<Point> PickIslandParts(const Point& pt, const EarthSurface& earth_surface);

template<class T>
void CopyUnique(vector<T>& dst, const vector<T>& src, int& count);

int main() {

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }

  EarthSurface earth_surface;
  try {
    earth_surface.ReadFromStream(ifs);
  } catch (EarthSurface::Exception& ex) {
    cerr << "EarthSurface::ReadFromStream failed: "
      << ex.reason() << endl;
    ifs.close();
    return -1;
  }
  ifs.close();

  ofstream ofs(OutputFilename);
  if (!ofs.good()) {
    cerr << "Can't create file: " << OutputFilename << endl;
    return -1;
  }

  ofs << CountIslands(earth_surface);
  ofs.close();

  return 0;
}

class PointBelongsToIsland 
  : public binary_function<Point, Island, bool> {
public:
  bool operator()(const Point& p, const Island& island) const {
    return island.Contains(p);
  }
};

int CountIslands(const EarthSurface& earth_surface) {
  vector<Island> islands;
  int rows_count = earth_surface.GetRowsCount();
  int cols_count = earth_surface.GetColsCount();
  int row = 0;
  while (row < rows_count) {
    int col = 0;
    while (col < cols_count) {
      Point pt(row, col);
      if (earth_surface.IsLand(pt)) {

        if (find_if(islands.begin(), islands.end(), 
              bind1st(PointBelongsToIsland(), pt)) == islands.end()) {
          /* find all land parts around point,
           if it hasn't belonged any island yet */
          vector<Point> parts = PickIslandParts(pt, earth_surface);
          Island island;
          for (size_t i = 0; i < parts.size(); ++i)
            island.AddPart(parts[i]);
          islands.push_back(island);
        } // if (find_if(islands.begin(), islands.end(), 

      } // if (earth_surface.IsLand(pt))
      ++col;
    } // while (col < cols_count)
    ++row;
  } // while (row < rows_count)
  return static_cast<int>(islands.size());
}
vector<Point> GetNeighborhood(const Point& pt, const EarthSurface& earth_surface) {
  vector<Point> neighborhood;

  if (earth_surface.IsLand(pt)) {
    Point p;
    p = Point(pt.row(), pt.col() - 1);
    if (p.col() >= 0 && p.col() < earth_surface.GetColsCount() &&
        p.row() >= 0 && p.row() < earth_surface.GetRowsCount() &&
        earth_surface.IsLand(p)) {
      neighborhood.push_back(p);
    }
    p = Point(pt.row(), pt.col() + 1);
    if (p.col() >= 0 && p.col() < earth_surface.GetColsCount() &&
        p.row() >= 0 && p.row() < earth_surface.GetRowsCount() &&
        earth_surface.IsLand(p)) {
      neighborhood.push_back(p);
    }
    p = Point(pt.row() - 1, pt.col());
    if (p.col() >= 0 && p.col() < earth_surface.GetColsCount() &&
        p.row() >= 0 && p.row() < earth_surface.GetRowsCount() &&
        earth_surface.IsLand(p)) {
      neighborhood.push_back(p);
    }
    p = Point(pt.row() + 1, pt.col());
    if (p.col() >= 0 && p.col() < earth_surface.GetColsCount() &&
        p.row() >= 0 && p.row() < earth_surface.GetRowsCount() &&
        earth_surface.IsLand(p)) {
      neighborhood.push_back(p);
    }
  } // if (earth_surface.IsLand(pt))
  
  return neighborhood;
}

vector<Point> PickIslandParts(const Point& pt, const EarthSurface& earth_surface) {
  vector<Point> seed_points = GetNeighborhood(pt, earth_surface);
  vector<Point> work_points;

  bool done = false;
  while (!done) {
    /* get new land parts at each iteration, until no one part will be got */
    int vain_step_count = 0;
    for (size_t i = 0; i < seed_points.size(); ++i) {
      vector<Point> step_points = GetNeighborhood(seed_points[i], earth_surface);

      /* exclude initial point from iteration results */
      vector<Point>::iterator it = find(step_points.begin(), step_points.end(), pt);
      if (it != step_points.end())
        step_points.erase(it);

      int count = 0;
      CopyUnique(work_points, step_points, count);
      /* step, based on i-th seed hasn't given us any new part */
      if (count == 0)
        ++vain_step_count;
    } // for (size_t i = 0; i < seed_points.size(); ++i)
    /* stop, if all steps were fruitless */
    done = (vain_step_count == static_cast<int>(seed_points.size()));
    seed_points = work_points;
  } // while (!done)

  work_points.push_back(pt);
  return work_points;
}

template<class T>
void CopyUnique(vector<T>& dst, const vector<T>& src, int& count) {
  count = 0;
  for (vector<T>::const_iterator it = src.begin(); it != src.end(); ++it) {
    if (find(dst.begin(), dst.end(), *it) == dst.end()) {
      dst.push_back(*it);
      ++count;
    }
  } // for (vector<T>::const_iterator it = src.begin(); it != src.end(); ++it)
}
