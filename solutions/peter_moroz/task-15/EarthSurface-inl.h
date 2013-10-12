#ifndef EARTH_SURFACE_INL_H_
#define EARTH_SURFACE_INL_H_

inline int EarthSurface::GetRowsCount() const {
  return static_cast<int>(layout_.size());
}
inline int EarthSurface::GetColsCount() const {
  return 
    layout_.size() > 0
    ? static_cast<int>(layout_[0].size())
    : 0;
}
// unsafe method, without bounds checking
inline bool EarthSurface::IsLand(const Point& pt) const {
  return (layout_[pt.row()][pt.col()] == 'o');
}

#endif // EARTH_SURFACE_INL_H_
