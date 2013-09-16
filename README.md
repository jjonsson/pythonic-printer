pythonic-printer
================

pythonic_printer.h provides a python-inspired print function (pp::print). This 
project is primarily for the purpose of exploring C++11 features.

Call pp::print<...>(...) on any number of objects of any types. It will print
them to std::cout in a manner similar to Python's print function. If a type that
supports iterators is used, it will be printed out in initializer list syntax.
Tuples and pairs will be printed out in Python tuple syntax. C-style arrays will
be printed like types that support iterators.

Run make in this directory to produce a small test program that show's off some
off pp::print's capabilities. The program will be called pptest.

gcc is required to compile pythonic_print. pythonic_print was developed to target
gcc 4.6.3 and may not work with any other version.
