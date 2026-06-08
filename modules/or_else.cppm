// Module-based compilation for C++20
// When compiled as a C++20 module, this exports the template interface
// When compiled as traditional headers, this file is not used

module;

#include <tyghbn/or_else.hpp>

export module tyghbn.or_else;

export namespace tyghbn {
  using tyghbn::or_else;
}
