// Module-based compilation for C++20
// When compiled as a C++20 module, this exports the template interface
// When compiled as traditional headers, this file is not used

module;

#include <tyghbn/add_one.hpp>

export module tyghbn.add_one;

export namespace tyghbn {
  using tyghbn::add_one;
}
