# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright 2018 Erez Geva

define c_cmp
cmp_c_$1: cmp_c.c
	$(CC) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o $$@

cmp_gnuc_$1: cmp_c.c
	$(CC) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o $$@

cmp_c_lang_$1: cmp_c.c
	$(CCLANG) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o $$@

cmp_gnuc_lang_$1: cmp_c.c
	$(CCLANG) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o $$@

endef
define cpp_cmp
cmp_cpp_$1: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -std=c++$1 $$< $(LDLIBS) -o $$@

cmp_gnucpp_$1: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -std=gnu++$1 $$< $(LDLIBS) -o $$@

cmp_cpp_lang_$1: cmp_cpp.cpp
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=c++$1 $$< $(LDLIBS) -o $$@

cmp_gnucpp_lang_$1: cmp_cpp.cpp
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=gnu++$1 $$< $(LDLIBS) -o $$@

endef

CCLANG:=clang
CXXLANG:=clang++
CPPFLAGS:=-Wall -Wextra -Winline -Werror -g
# We do not care about diagnostic of 'register' keyword
# We skip it in C++17 and above
CLANG_FLAGS:=-Wno-deprecated-register
LDLIBS:=-lm
C_STD:=89 99 11 17 2x
CPP_STD:=98 11 14 17 20
ALL:=$(addprefix cmp_c_,$(C_STD) 95 ansi)\
     $(foreach n,$(C_STD),cmp_gnuc_$(n) cmp_c_lang_$(n) cmp_gnuc_lang_$(n))\
     $(addprefix cmp_cpp_,$(CPP_STD) 23 ansi skip)\
     $(addprefix cmp_gnucpp_,$(CPP_STD) 23)\
     $(foreach n,$(CPP_STD),cmp_cpp_lang_$(n) cmp_gnucpp_lang_$(n))\
     utest

all: $(ALL)
clean:
	$(RM) $(ALL) *.o

$(eval $(foreach n,$(C_STD),$(call c_cmp,$n)))
$(eval $(foreach n,$(CPP_STD) 23,$(call cpp_cmp,$n)))
cmp_c_ansi: cmp_c.c
	$(CC) $(CPPFLAGS) -ansi $< $(LDLIBS) -o $@
cmp_c_95: cmp_c.c
	$(CC) $(CPPFLAGS) -std=iso9899:199409 $< $(LDLIBS) -o $@
cmp_cpp_ansi: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -ansi $< $(LDLIBS) -o $@
cmp_cpp_skip: cmp_cpp.cpp
	$(CXX) $(CPPFLAGS) -DST_SKIP_REGISTER -std=c++98 $< $(LDLIBS) -o $@

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
