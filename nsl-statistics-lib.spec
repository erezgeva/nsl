# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
# RPM specification file for libptpmgmt rpm packages
###############################################################################
Name:           nsl-statistics-lib
Version:        0.5
Release:        1%{?dist}
URL:            https://github.com/erezgeva/nsl
BuildRequires:  discount cmark
BuildRequires:  doxygen graphviz texlive-epstopdf pandoc
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
