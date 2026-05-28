// Module-based compilation for C++20
// When compiled as a C++20 module, this exports the template interface
// When compiled as traditional headers, this file is not used

module;

#include "sorted_array_tower/sorted_array_tower.hpp"

export module sorted_array_tower;

export namespace sorted_array_tower {
  using sorted_array_tower::SortedArrayTower;
}
