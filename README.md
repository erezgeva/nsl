<!-- SPDX-License-Identifier: GFDL-1.3-no-invariants-or-later
     SPDX-FileCopyrightText: Copyright © 2023 Erez Geva <ErezGeva2@gmail.com> -->
# Network Statistics Library

This project provide a small library to calculate statistics of a set.
The library have the following properties

 1. The calculation is done while the program is adding new values to the set.  
 So we get the current statistics ***without*** stopping.
 1. The library is a header file, no need to link, just include the header.  
 The header is under MIT license.
 1. The library was design with network applications in mind,  
 which requires calculate statistics on the go,  
 the library can be used by any other discipline.
 1. The make file provides testing and Debian packaging,  
 the library header do not require any build.
 1. The library header support both C and C++.  
 C++ application can use C++ classes or optionally C structure.

# Macros to use with the Library

|Macro name |C           |C++         |C Default   |C++ Default|Explanation                 |
|-----------|------------|------------|------------|-----------|----------------------------|
|ST_USE_C   |C structures|C structures|C structures|C++ Classes|Force C++ using C structures|
|ST_USE_TYPE|set type    |set type    |Use int64_t |Use int64_t|Element in set type         |

ST_SKIP_REGISTER skip using the 'register' keyword.
