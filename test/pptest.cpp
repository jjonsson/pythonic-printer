#include "pythonic_print.h"
#include <tuple>
#include <unordered_map>

using namespace std;
using namespace pp;

int main()
{
  unordered_map<int, string> mymap{make_pair(42, "foo"), make_pair(5, "bar")};
  print("hello", "world", 42, 3.14);
  print(make_tuple("tuples", "inside", make_tuple("tuples")));
  print(mymap, "weeeeeeeeeeeee");
  int myarray[3] = {1, 2, 3};
  print(myarray);
}
