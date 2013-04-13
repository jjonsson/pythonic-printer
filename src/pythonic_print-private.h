// pythonic_print-private.h
// copyright 2013 James P. Jonsson

#include <iostream>
#include <type_traits>
#include <utility>
#include <tuple>
#include <typeinfo>

using namespace std;

namespace pp
{
  //
  // static highest-level functions
  //

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
    decltype(stream << declval<T>(), bool())
  {
    return true;
  }

  // fallback
  template<typename T>
  static constexpr bool has_stream_insert(...)
  {
    return false;
  }


  //
  // internal class declarations
  //

  // Note: The lack of partial template specialization for functions in C++
  //   means that a class is needed instead of just functions.

  // does all the heavy lifting for print
  template<typename T>
  class PrintHelper
  {
  public:
    // print a single object to the stream
    static void print_object(ostream&, const T&);
  };

  // specialization for pairs
  template<typename T1, typename T2>
  class PrintHelper<pair<T1, T2>>
  {
  public:
    static void print_object(ostream&, const pair<T1, T2>&);
  };

  // specialization for tuples
  template<typename... Ts>
  class PrintHelper<tuple<Ts...>>
  {
  public:
    static void print_object(ostream&, const tuple<Ts...>&);
  };


  //
  // member function definitions
  //

  // if operator<<(ostream, T) is defined, just use that
  // however, if T is an array, we don't want to just print the address, so
  //   use a different definition for arrays
  // if the array is a C-style string, we do want to use the << operator
  template<typename T>
  auto PrintHelper<T>::print_object(ostream& stream, const T& object) ->
    typename enable_if<has_stream_insert<T>(0)
                    && (!is_array<T>::value
                    ||  is_convertible<T, string>::value)>::type
  {
    stream << object;
  }

  // T is an array
  template<typename T>
  auto PrintHelper<T>::print_object(ostream& stream, const T& object) ->
    typename enable_if<is_array<T>::value
                    && (!is_convertible<T, string>::value)>::type
  {
    // just in case a null pointer gets passed in, though I doubt a null
    //   pointer can ever be passed into this function because of the enable_if
    if (object == NULL) stream << "{}";

    stream << "{";
    PrintHelper<decltype(object[0])>::print_object(object[0]);
    for (int i = 1; i < extent<T>::value; ++i) {
      stream << ", ";
      PrintHelper<decltype(object[0])>::print_object(object[i]);
    }
    stream << "}";
  }

  // fallback case for when there is not a simple way to print the given type
  template<typename T>
  auto PrintHelper<T>::print_object(ostream& stream, const T& object) ->
    typename enable_if<(!has_stream_insert<T>(0))
                    && (!has_begin_and_end<T>(0))>::type
  {
    stream << "[[X]]";
  }

  // if the given type has iterators, print it using initializer list syntax
  template<typename T>
  auto PrintHelper<T>::print_object(ostream& stream, const T& object) ->
    typename enable_if<(has_begin_and_end<T>(0))
                    && (!has_stream_insert<T>(0))>::type
  {
    auto it = object.begin();
    stream << "{";
    if (it != object.end()) {
      PrintHelper<typename T::value_type>::print_object(*it);
    }
    for (++it; it != object.end(); ++it) {
      stream << ", ";
      PrintHelper<typename T::value_type>::print_object(*it);
    }
    stream << "}";
  }

  // base case when the index of the element we are printing is outside the
  //   range of printable elements
  template<typename T, int I>
  static auto print_tuple(ostream& stream, const T& t) ->
    typename enable_if<I >= tuple_size<T>::value>::type {}

  // prints element I of a tuple of type T
  // element 0 should be printed with PrintHelper::print before this function
  //   is called on element 1
  template<typename T, int I>
  static auto print_tuple(ostream& stream, const T& t) ->
    typename enable_if<I < tuple_size<T>::value>::type
  {
    stream << ", ";
    PrintHelper<typename tuple_element<I, T>::type>::print_object(get<I>(t));
    print_tuple<T, I + 1>(t);
  }

  // printing a pair is pretty easy
  template<typename T1, typename T2>
  void PrintHelper<pair<T1, T2>>::print_object(ostream& stream,
                                               const pair<T1, T2>& t)
  {
    stream << "(";
    PrintHelper<T1>::print_object(get<0>(t));
    stream << ", ";
    PrintHelper<T2>::print_object(get<1>(t));
    stream << ")";
  }

  // print element 0 then call print_tuple on the rest of the tuple
  // this is to print the correct number of commas
  template<typename... Ts>
  void PrintHelper<tuple<Ts...>>::print_object(ostream& stream,
                                               const tuple<Ts...>& t)
  {
    stream << "(";
    // if the tuple is empty, just print parens
    if (tuple_size<tuple<Ts...>>::value > 0) {
      PrintHelper<typename tuple_element<0, tuple<Ts...>>::type>::
        print(get<0>(t));
    }
    print_tuple<tuple<Ts...>, 1>(t);
    stream << ")";
  }


  //
  // highest-level external functions
  //

  // base case
  void print(ostream& stream)
  {
    stream << endl;
  }

  // change the stream we print to
  template<typename T, typename... Ts>
  void print(ostream& stream, const T& first, const Ts&... rest)
  {
    PrintHelper<T>::print_object(stream, first);
    stream << " ";
    print<Ts...>(stream, rest...);
  }

  // when no stream is given, default to cout
  template<typename... Ts>
  void print(const Ts&... all)
  {
    print<Ts...>(cout, all...);
  }


} // end namespace pp
