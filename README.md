# Network Statistics Library

This project provide a small library to calculate statistics of a set.
The library have the following properties

 1. The calculation is done while the program is adding new values to the set. So we get the current statistics *** without *** stoping.
 2. The library is a header file, no need to link, just include it. The header is under MIT license.
 3. The library was design for network, where we need to run statistics on the go, but could be used by any application.
 4. The make file provides testing and Debian packaging, no need to build the library itself.
 5. The library header can be used with structure in C and C++, or with classes in C++.

# Macroes to use with the Library
<br>

|Macro name |C          |C++        |C Default   |C++ Default |Explenation                 |
|-----------|-----------|-----------|------------|------------|----------------------------|
|ST_USE_C   |C structres|C structres|C structres |C++ Clases  |Force C++ using C structres |
|ST_USE_TYPE|set type   |set type   |Use int64_t |Use int64_t |Element in set type         |
