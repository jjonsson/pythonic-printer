// pythonic_print.h
// copyright 2013 James P. Jonsson

#ifndef PYTHONIC_PRINT_H
#define PYTHONIC_PRINT_H

#include <ostream>

namespace pp {
  // This print function prints in a style similar to Python's print.
  // Pass in whatever you want!
  template<typename... Ts>
  void print(const Ts&...);

  // To print to a stream other than std::cout, pass it in as the first
  //   argument. If C++ had named arguments it would be more pythonic.
  template<typename... Ts>
  void print(std::ostream&, const Ts&...);
}

#include "pythonic_print-private.h"

#endif
