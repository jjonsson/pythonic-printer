#ifndef PYTHONIC_PRINT_H
#define PYTHONIC_PRINT_H

namespace pp {
  template<typename... Ts>
  void print(Ts...);
}

#include "pythonic_print-private.h"

#endif