#include <tuple>
#include <unordered_map>
#include <type_traits>

#include "pythonic_print.h"

using namespace std;
using namespace pp;

int main()
{
  print("hello", "world", 42, 3.14);

  print(make_tuple("tuples", "inside", make_tuple("tuples")));

  print();

  unordered_map<int, string> mymap{make_pair(42, "foo"), make_pair(5, "bar")};
  print(mymap, "weeeeeeeeeeeee");

  int myarray[3] = {1, 2, 3};
  print(myarray);

  // only the address will be printed
  int* myheaparray = new int[5];
  myheaparray[0] = 1;
  myheaparray[4] = 5;
  print(myheaparray);
}
