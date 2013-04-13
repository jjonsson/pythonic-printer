#include "pythonic_print.h"
#include <iostream>

namespace pp {
  using namespace std;

  // base cases of print
  void print(ostream& stream)
  {
    stream << endl;
  }

  void print()
  {
    cout << endl;
  }

} // end namespace pp
