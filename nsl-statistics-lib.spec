# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
# RPM specification file for libptpmgmt rpm packages
###############################################################################
Name:           nsl-statistics-lib
Version:        1.0
Release:        1%{?dist}
URL:            https://github.com/erezgeva/nsl
BuildRequires:  doxygen graphviz texlive-epstopdf discount
License:        GPL-3.0-or-later
#Source0:        https://github.com/erezgeva/nsl/archive/refs/tags/%%{version}.tar.gz
Source0:        %{name}-%{version}.txz
Summary:        library to calculate statistics while gathering measurements.
%description
Library to calculate statistics while gathering measurements

%package        devel
Summary:        The statistics nsl library header file
License:        MIT
BuildArch:      noarch
Provides:       %{name}-static = %{version}-%{release}
%description    devel
The %{name}-devel package contains the library header file.

%package        doc
Summary:        Documentation files for the statistics nsl library
License:        GFDL-1.3-no-invariants-or-later
BuildArch:      noarch
%description    doc
The statistics nsl library documentation

%prep
%setup -q

%build
%make_build --no-print-directory doxygen

%install
%make_install --no-print-directory

%files devel
%{_includedir}/*

%files doc
%{_datadir}/doc/%{name}-doc/*

%changelog
* Thu Aug 13 2026 ErezGeva2@gmail.com 1.0-1
- Add README and add it to the documents package
- Add GitHub pages action to upload the Doxygen
- Add GitHub continuous integration in
  - Debian
  - Fedora
  - Arch Linux
  - MacOS
  - Windows MingW64
  - Windows MSVC
- Add header development and documents packages to
  - Debian
  - Fedora/RPM
  - Arch Linux/pacman
- Add Doxygen documents
  - Add comments in nsl_statistics.h
  - Filter nsl_statistics.h to a C and C++ bases to generate the Doxygen
- Update reuse files
- Add main License file
- Add SPDX tags for license and copyright
- Improve nsl_statistics.h
  - Add flag 'ST_SKIP_REGISTER' to prevent
    the use of 'register` keyword
  - Improve the selection of the square root function
  - Dived by 2 to detect floating
  - Syntax improvements
- Add compilation
  - newer C and C++ standards
  - with clang
- Improve documentation and spelling
- Add test compilation for C++ 17
  `register` keyword is obsolete in C++ 17
- Comply to FSFE reuse recommendations
- Add Debian git ignore for Debian package build
- Add unit test
- Add compilation of different standards
- Follow limit constants defined by standard
- Use include headers according to standard
  C for C and C++ for C++
- Add missing clearing of the vector on allocation

* Fri Mar 22 2019 ErezGeva2@gmail.com 0.5-1
- Fix add_elem.
- copy return Boolean to indicate if it was copying.
- copy avoid copying same structure.

* Thu Mar 21 2019 ErezGeva2@gmail.com 0.4-1
- Add a proper casting to the LDBL_MAX macro

* Tue Mar 05 2019 ErezGeva2@gmail.com 0.3-1
- Add statistics vector

* Thu Dec 06 2018 ErezGeva2@gmail.com 0.2-1
- Remove the max and min macros, may conflict with C++ standard functions.

* Mon Sep 24 2018 ErezGeva2@gmail.com 0.1-1
- Initial release.
