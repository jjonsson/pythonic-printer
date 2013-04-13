pythonic-printer
================

pythonic_printer.h provides a python-inspired print function (pp::print). I 
wrote this function mostly to learn some C++11 features.

Call pp::print<...>(...) on any number of objects of any types. It will print
them to std::cout in a manner similar to Python's print function. If a type that
supports iterators is used, it will be printed out in initializer list syntax.
Tuples and pairs will be printed out in Python tuple syntax.

Run make in this directory to produce a small test program that show's off some
off pp::print's capabilities.

gcc is required to compile pythonic_print. It should work with versions 4.6 and
higher.