// pythonic_print-private.h
// copyright 2013 James P. Jonsson

#include <iostream>
#include <type_traits>
#include <utility>
#include <tuple>

using namespace std;

namespace pp
{
  // uses SFINAE to determine if the type supports iterators
  template<typename T>
  static constexpr auto has_begin_and_end(int x) ->
    decltype(declval<T>().begin(), declval<T>().end(), bool())
  {
    return true;
  }

  // ellipsis has the lowest priority in function selection, so this function
  //   will be chosen if and only if the above function fails to be defined
  template<typename T>
  static constexpr bool has_begin_and_end(...)
  {
    return false;
  }

  // if the type has the stream insert operator, <<, we can use it to print
  template<typename T>
  static constexpr auto has_stream_insert(int x) ->
    decltype(cout << declval<T>(), bool())
  {
    return true;
  }

  // fallback
  template<typename T>
  static constexpr bool has_stream_insert(...)
  {
    return false;
  }


  // I tried to use SFINAE to print tuples and pairs with a single function,
  //   but I couldn't get that to work, so I used the following template class
  //   structure with specialization for pairs and tuples instead.

  // a class with the sole purpose of containing PrintHelper<T>::print(T)
  template<typename T>
  class PrintHelper
  {
  public:
    // a helper function that is mutually recursive with print_helper and
    //   print_tuple
    static void print(T);
  };

  // specialization for pairs
  template<typename T1, typename T2>
  class PrintHelper<pair<T1, T2>>
  {
  public:
    static void print(pair<T1, T2> t);
  };

  // specialization for tuples
  template<typename... Ts>
  class PrintHelper<tuple<Ts...>>
  {
  public:
    static void print(tuple<Ts...> t);
  };

  // if operator<<(ostream, T) is defined, just use that
  template<typename T>
  static auto print_helper(T first) ->
    typename enable_if<has_stream_insert<T>(0)
                    && (!is_array<T>::value)>::type
  {
    cout << first;
  }

  // T is an array
  template<typename T>
  static auto print_helper(T object) ->
    typename enable_if<is_array<T>::value>::type
  {
    cerr << "got to array printer" << endl;
    // just in case a null pointer gets passed in, though I doubt a null pointer
    //   can ever be passed into this function because of the enable_if
    if (object == NULL) cout << "{}";

    int length = sizeof(object) / sizeof(object[0]);

    cout << "{";
    PrintHelper<typename T::value_type>::print(object[0]);
    for (int i = 1; i < length; ++i) {
      cout << ", ";
      PrintHelper<typename T::value_type>::print(object[i]);
    }
    cout << "}";
  }

  // fallback case for when there is not a simple way to print the given type
  template<typename T>
  static auto print_helper(T first) ->
    typename enable_if<(!has_stream_insert<T>(0))
                    && (!has_begin_and_end<T>(0))>::type
  {
    cout << "[[X]]";
  }

  // if the given type has iterators, print it using initializer list syntax
  template<typename T>
  static auto print_helper(T first) ->
    typename enable_if<(has_begin_and_end<T>(0))
                    && (!has_stream_insert<T>(0))>::type
  {
    auto it = first.begin();
    cout << "{";
    if (it != first.end()) {
      PrintHelper<typename T::value_type>::print(*it);
    }
    for (++it; it != first.end(); ++it) {
      cout << ", ";
      PrintHelper<typename T::value_type>::print(*it);
    }
    cout << "}";
  }

  // when called on a type that is not a pair or tuple, we can let print_helper
  //   do all the work
  template<typename T>
  void PrintHelper<T>::print(T t)
  {
    print_helper<T>(t);
  }

  // base case when the index of the element we are printing is outside the
  //   range of printable elements
  template<typename T, int I>
  static auto print_tuple(T t) ->
    typename enable_if<I >= tuple_size<T>::value>::type {}

  // prints element I of a tuple of type T
  // element 0 should be printed with PrintHelper::print before this function
  //   is called on element 1
  template<typename T, int I>
  static auto print_tuple(T t) ->
    typename enable_if<I < tuple_size<T>::value>::type
  {
    cout << ", ";
    PrintHelper<typename tuple_element<I, T>::type>::print(get<I>(t));
    print_tuple<T, I + 1>(t);
  }

  // printing a pair is pretty easy
  template<typename T1, typename T2>
  void PrintHelper<pair<T1, T2>>::print(pair<T1, T2> t)
  {
    cout << "(";
    PrintHelper<T1>::print(get<0>(t));
    cout << ", ";
    PrintHelper<T2>::print(get<1>(t));
    cout << ")";
  }

  // print element 0 then call print_tuple on the rest of the tuple
  // this is to print the correct number of commas
  template<typename... Ts>
  void PrintHelper<tuple<Ts...>>::print(tuple<Ts...> t)
  {
    cout << "(";
    // if the tuple is empty, just print parens
    if (tuple_size<tuple<Ts...>>::value > 0) {
      PrintHelper<typename tuple_element<0, tuple<Ts...>>::type>::
        print(get<0>(t));
    }
    print_tuple<tuple<Ts...>, 1>(t);
    cout << ")";
  }

  // base case
  template<typename T>
  void print(T first)
  {
    PrintHelper<T>::print(first);
    cout << endl;
  }

  // accepts any number/combination of arguments and types of arguments
  template<typename T, typename... Ts>
  void print(T first, Ts... rest)
  {
    PrintHelper<T>::print(first);
    cout << " ";
    print<Ts...>(rest...);
  }

} // end namespace pp
