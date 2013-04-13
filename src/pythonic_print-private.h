// pythonic_print-private.h
// copyright 2013 James P. Jonsson

#include <iostream>
#include <type_traits>
#include <utility>
#include <tuple>

namespace pp
{

  // uses SFINAE to determine if the type supports iterators
  template<typename T>
  static constexpr auto has_begin_and_end(int x) ->
    decltype(std::declval<T>().begin(), std::declval<T>().end(), bool())
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
    decltype(std::cout << std::declval<T>(), bool())
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
  class PrintHelper<std::pair<T1, T2>>
  {
  public:
    static void print(std::pair<T1, T2> t);
  };

  // specialization for tuples
  template<typename... Ts>
  class PrintHelper<std::tuple<Ts...>>
  {
  public:
    static void print(std::tuple<Ts...> t);
  };

  // if operator<<(ostream, T) is defined, just use that
  template<typename T>
  static auto print_helper(T first) ->
    typename std::enable_if<has_stream_insert<T>(0)>::type
  {
    std::cout << first;
  }

  // fallback case for when there is not a simple way to print the given type
  template<typename T>
  static auto print_helper(T first) ->
    typename std::enable_if<(not has_stream_insert<T>(0))
                        and (not has_begin_and_end<T>(0))>::type
  {
    std::cout << "[[X]]";
  }

  // if the given type has iterators, print it using initializer list syntax
  template<typename T>
  static auto print_helper(T first) ->
    typename std::enable_if<(has_begin_and_end<T>(0))
                        and (not has_stream_insert<T>(0))>::type
  {
    auto it = first.begin();
    std::cout << "{";
    if (it != first.end()) {
      PrintHelper<typename T::value_type>::print(*it);
    }
    for (++it; it != first.end(); ++it) {
      std::cout << ", ";
      PrintHelper<typename T::value_type>::print(*it);
    }
    std::cout << "}";
  }

  // when called on a type that is not a pair or tuple, we can let print_helper
  //   do all the work
  template<typename T>
  void PrintHelper<T>::print(T t)
  {
    print_helper<T>(t);
  }

  // base case when the index of the element we are printing is outside the range
  //   of printable elements
  template<typename T, int I>
  static auto print_tuple(T t) ->
    typename std::enable_if<I >= std::tuple_size<T>::value>::type {}

  // prints element I of a tuple of type T
  // element 0 should be printed with PrintHelper::print before this function
  //   is called on element 1
  template<typename T, int I>
  static auto print_tuple(T t) ->
    typename std::enable_if<I < std::tuple_size<T>::value>::type
  {
    std::cout << ", ";
    PrintHelper<typename std::tuple_element<I, T>::type>::print(std::get<I>(t));
    print_tuple<T, I + 1>(t);
  }

  // printing a pair is pretty easy
  template<typename T1, typename T2>
  void PrintHelper<std::pair<T1, T2>>::print(std::pair<T1, T2> t)
  {
    std::cout << "(";
    PrintHelper<T1>::print(std::get<0>(t));
    std::cout << ", ";
    PrintHelper<T2>::print(std::get<1>(t));
    std::cout << ")";

  }

  // print element 0 then call print_tuple on the rest of the tuple
  // this is to print the correct number of commas
  template<typename... Ts>
  void PrintHelper<std::tuple<Ts...>>::print(std::tuple<Ts...> t)
  {
    std::cout << "(";
    // if the tuple is empty, just print parens
    if (std::tuple_size<std::tuple<Ts...>>::value > 0) {
      PrintHelper<typename std::tuple_element<0, std::tuple<Ts...>>::type>::print(std::get<0>(t));
    }
    print_tuple<std::tuple<Ts...>, 1>(t);
    std::cout << ")";

  }

  // base case
  template<typename T>
  void print(T first)
  {
    PrintHelper<T>::print(first);
    std::cout << std::endl;
  }

  // accepts any number/combination of arguments and types of arguments
  template<typename T, typename... Ts>
  void print(T first, Ts... rest)
  {
    PrintHelper<T>::print(first);
    std::cout << " ";
    print<Ts...>(rest...);
  }

} // end namespace pp
