# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright 2018 Erez Geva

CPPFLAGS:=-Wall -Wextra -Winline -g
LDLIBS:=-lm
ALL=cmp_c_ansi cmp_c_89 cmp_c_99 cmp_cpp_ansi cmp_cpp_98 cmp_cpp_11 utest
all: $(ALL)
clean:
	$(RM) $(ALL) *.o

cmp_c_ansi: cmp_c.c
	$(CC) $(CPPFLAGS) -ansi $< $(LDLIBS) -o $@
cmp_c_89: cmp_c.c
	$(CC) $(CPPFLAGS) -std=c89 $< $(LDLIBS) -o $@
cmp_c_99: cmp_c.c
	$(CC) $(CPPFLAGS) -std=c99 $< $(LDLIBS) -o $@
cmp_cpp_ansi: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -ansi $< $(LDLIBS) -o $@
cmp_cpp_98: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -std=c++98 $< $(LDLIBS) -o $@
cmp_cpp_11: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -std=c++11 $< $(LDLIBS) -o $@

utest.o:
	printf 'int main(int argc,char**argv)%s'\
	 '{::testing::InitGoogleTest(&argc,argv);return RUN_ALL_TESTS();}' |\
	$(CXX) $(CPPFLAGS) -include gtest/gtest.h -DGTEST_HAS_PTHREAD=1\
	 -c -x c++ - -o $@

utest: utest.o utest_cpp.o utest_c.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS)\
	 -lgtest -lpthread -o $@

deb:
	dpkg-buildpackage -b --no-sign
