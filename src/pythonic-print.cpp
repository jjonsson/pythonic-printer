#include "pythonic-print.h"
#include <iostream>

namespace pp {
  using namespace std;

  // base cases of print
  void print(ostream& stream)
  {
    stream << sendl;
  }

  void print()
  {
    cout << endl;
  }

} // end namespace pp