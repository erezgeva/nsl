<!-- SPDX-License-Identifier: GFDL-1.3-no-invariants-or-later
     SPDX-FileCopyrightText: Copyright © 2023 Erez Geva <ErezGeva2@gmail.com> -->
# Network Statistics Library

The `nsl_statistics.h` header contains two classes.

- `class nsl_stats`
  - The class accumulate elements by adding new elements  
    with `add_elem()` on the run.
  - The class provide mean, average and standard deviation.
  - The calculation is every time a new element is added,  
    so the user can have the statistics on the run.
  - So no need to store the elements themselves,  
    only the accumulated statistics is kept, 5 variable plus 3 for under cut statistics
  - The class support a down cut,  
    a start number of elements on with the class will calculate a separate statistics,  
    some measurement are prone to turbulence during the start,  
    so the user can ignore them by using the cut number to start from.
- `class nsl_vec_stats`
  - The class provide a vector of fixed size cells storing elements of values.
  - Each cell store a range of values, with addition of a bellow cell and above cell,  
    storing values outside the vector.
  - The purpose is to draw a graph representing the distribution of elements values,  
    as each cell count is the piller height in the graph.

# Preface

This project provide a small library to calculate statistics on the run.  
The library have the following properties

1. The calculation is done while the program is adding new values.  
   So we get the current statistics ***without*** stopping.
1. The library is a header file, no need to link, just include the header.  
   The header is under MIT license.
1. The library was design with network applications in mind,  
   which requires calculate statistics on the go,  
   the library can be used by any other discipline.
1. The make file provides testing and packaging,  
   the library header do not require any build.
1. The library header support both C and C++.  
   C++ application can use C++ classes or optionally C structure.

# Macros to use with the Library

| Macro name  | C            | C++          | C Default    | C++ Default | Explanation                  |
|-------------|--------------|--------------|--------------|-------------|------------------------------|
| ST_USE_C    | C structures | C structures | C structures | C++ Classes | Force C++ using C structures |
| ST_USE_TYPE | set type     | set type     | Use int64_t  | Use int64_t | Element in set type          |

ST_MEAN_TYPE_DOUBLE use `double` instead of `long double` to calculate mean values.  
ST_SKIP_REGISTER prevent using the `register` keyword.

# Packaging

The project comes with packaging:

- Debian packages for Debian and Debian based Linux systems.
- RPM packages for Red Hat based Linux systems.
- Arch Linux packages.

# API Documentation

The header API generated with Doxygen, can be generated using the make-file.  
And can be found here:

- [C API](https://erezgeva.github.io/nsl/c/html)
- [C++ API](https://erezgeva.github.io/nsl/cpp/html)

# Licence

The Licence of this project is

- The `nsl_statistics.h` header is under MIT.
- Documentation is under under GFDL v1.3 or later without invariants sections.
- The testing files are under GPL v3 or later.
