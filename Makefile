# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2018 Erez Geva <ErezGeva2@gmail.com>

ver_maj:=0
ver_min:=5

OUT:=out
HCFG:=config.h
which=$(shell which $1 2>/dev/null)

define c_cmp
$(OUT)/cmp_c_$1: cmp_c.c | $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnuc_$1: cmp_c.c | $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_c_lang_$1: cmp_c.c | $(HCFG) $(OUT)
	$(CCLANG) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnuc_lang_$1: cmp_c.c | $(HCFG) $(OUT)
	$(CCLANG) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o "$$@"

endef
define cpp_cmp
$(OUT)/cmp_cpp_$1: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -std=c++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnucpp_$1: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -std=gnu++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_cpp_lang_$1: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=c++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnucpp_lang_$1: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=gnu++$1 $$< $(LDLIBS) -o "$$@"

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
ALL:=$(addprefix $(OUT)/cmp_c_,$(C_STD) 95 ansi)\
     $(foreach n,$(C_STD),$(OUT)/cmp_gnuc_$(n) $(OUT)/cmp_c_lang_$(n)\
                 $(OUT)/cmp_gnuc_lang_$(n))\
     $(addprefix $(OUT)/cmp_cpp_,$(CPP_STD) 23 ansi skip)\
     $(addprefix $(OUT)/cmp_gnucpp_,$(CPP_STD) 23)\
     $(foreach n,$(CPP_STD),$(OUT)/cmp_cpp_lang_$(n)\
                 $(OUT)/cmp_gnucpp_lang_$(n))\

all: $(ALL)
clean:
	$(RM) -rf $(OUT) *.o utest

.PHONY: all clean install deb srcpkg rpm

$(OUT) rpm/SOURCES:
	mkdir -p "$@"
$(HCFG):
	./probe.sh "$@"

$(eval $(foreach n,$(C_STD),$(call c_cmp,$n)))
$(eval $(foreach n,$(CPP_STD) 23,$(call cpp_cmp,$n)))
$(OUT)/cmp_c_ansi: cmp_c.c | $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -ansi $< $(LDLIBS) -o "$@"
$(OUT)/cmp_c_95: cmp_c.c | $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=iso9899:199409 $< $(LDLIBS) -o "$@"
$(OUT)/cmp_cpp_ansi: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -ansi $< $(LDLIBS) -o "$@"
$(OUT)/cmp_cpp_skip: cmp_cpp.cpp | $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -DST_SKIP_REGISTER -std=c++98 $< $(LDLIBS) -o "$@"

utest.o:
	printf 'int main(int argc,char**argv)%s'\
	 '{::testing::InitGoogleTest(&argc,argv);return RUN_ALL_TESTS();}' |\
	$(CXX) $(CPPFLAGS) -include gtest/gtest.h -DGTEST_HAS_PTHREAD=1\
	 -c -x c++ - -o "$@"

utest: utest.o utest_cpp.o utest_c.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS)\
	 -lgtest -lpthread -o "$@"

install:
	$(INSTALL) -m 644 -D nsl_statistics.h -t $(DESTDIR)/usr/include

deb:
	dpkg-buildpackage -b --no-sign

SRC_FILES:=nsl_statistics.h README.md Makefile LICENSE.txt
SRC_NAME:=nsl-statistics-lib-$(ver_maj).$(ver_min)
TAR:=tar cfJ
LIB_SRC:=$(SRC_NAME).txz
$(LIB_SRC): $(SRC_FILES)
	$(TAR) "$@" $^ --transform "s!^!$(SRC_NAME)/!S"
srcpkg: $(LIB_SRC)

ifneq ($(call which,rpmbuild),)
rpm: $(LIB_SRC) rpm/SOURCES
	cp $(LIB_SRC) rpm/SOURCES/
	rpmbuild --define "debug_package %nil" --define "_topdir $(PWD)/rpm" \
	 -bb nsl-statistics-lib.spec
endif # which rpmbuild
