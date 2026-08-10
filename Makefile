# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2018 Erez Geva <ErezGeva2@gmail.com>

ver_maj:=0
ver_min:=5

OUT:=out
HCFG:=config.h
PKG:=nsl-statistics-lib
HEADER:=nsl_statistics.h
which=$(shell which $1 2>/dev/null)

define c_cmp
$(OUT)/cmp_c_$1: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnuc_$1: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_c_lang_$1: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CCLANG) $(CPPFLAGS) -std=c$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnuc_lang_$1: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CCLANG) $(CPPFLAGS) -std=gnu$1 $$< $(LDLIBS) -o "$$@"

endef
define cpp_cmp
$(OUT)/cmp_cpp_$1: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -std=c++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnucpp_$1: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -std=gnu++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_cpp_lang_$1: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=c++$1 $$< $(LDLIBS) -o "$$@"

$(OUT)/cmp_gnucpp_lang_$1: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXXLANG) $(CPPFLAGS) $(CLANG_FLAGS) -std=gnu++$1 $$< $(LDLIBS) -o "$$@"

endef

CCLANG:=clang
CXXLANG:=clang++
CPPFLAGS:=-Wall -Wextra -Winline -Werror -g
ifdef ST_MEAN_TYPE_DOUBLE
CPPFLAGS+= -DST_MEAN_TYPE_DOUBLE
endif
# We do not care about diagnostic of 'register' keyword
# We skip it in C++17 and above
CLANG_FLAGS:=-Wno-deprecated-register
LDLIBS:=-lm
C_STD:=89 99 11 17 2x
CPP_STD:=98 11 14 17 20

all: all_gcc all_clang utest doxygen
all_gcc: $(addprefix $(OUT)/cmp_c_,$(C_STD) 95 ansi)\
         $(foreach n,$(C_STD),$(OUT)/cmp_gnuc_$(n))\
         $(addprefix $(OUT)/cmp_cpp_,$(CPP_STD) 23 ansi skip)\
         $(addprefix $(OUT)/cmp_gnucpp_,$(CPP_STD) 23)
all_clang: $(foreach n,$(C_STD),$(OUT)/cmp_c_lang_$(n) $(OUT)/cmp_gnuc_lang_$(n))\
           $(foreach n,$(CPP_STD),$(OUT)/cmp_cpp_lang_$(n)\
                       $(OUT)/cmp_gnucpp_lang_$(n))

clean:
	$(RM) -rf $(OUT) *.o utest config.h c cpp doc

.PHONY: all all_gcc all_clang doxygen clean install deb srcpkg rpm pkg\
        distclean clean_doxygen clean_deb clean_rpm clean_pkg

distclean: clean clean_doxygen clean_deb clean_rpm clean_pkg
	$(RM) -rf $(PKG)-*.txz

$(OUT) rpm/SOURCES:
	mkdir -p "$@"
$(HCFG):
	./probe.sh "$@"

$(eval $(foreach n,$(C_STD),$(call c_cmp,$n)))
$(eval $(foreach n,$(CPP_STD) 23,$(call cpp_cmp,$n)))
$(OUT)/cmp_c_ansi: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -ansi $< $(LDLIBS) -o "$@"
$(OUT)/cmp_c_95: cmp_c.c | $(HEADER) $(HCFG) $(OUT)
	$(CC) $(CPPFLAGS) -std=iso9899:199409 $< $(LDLIBS) -o "$@"
$(OUT)/cmp_cpp_ansi: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -ansi $< $(LDLIBS) -o "$@"
$(OUT)/cmp_cpp_skip: cmp_cpp.cpp | $(HEADER) $(HCFG) $(OUT)
	$(CXX) $(CPPFLAGS) -DST_SKIP_REGISTER -std=c++98 $< $(LDLIBS) -o "$@"

doxygen/c/$(HEADER): $(HEADER)
	mkdir -p "$(@D)"
	printf '' | $(CC) -pipe -E -CC -xc - -include $< -o /dev/stdout |\
	sed -n '/^$$/d;/^#/d;/ @file$$/,$$p' > "$@"
	astyle --project=none --suffix=none "$@"
doxygen/cpp/$(HEADER): $(HEADER)
	mkdir -p "$(@D)"
	printf '' | $(CXX) -pipe -E -CC -xc++ - -include $< -o /dev/stdout |\
	sed -n '/^$$/d;/^#/d;/ @file$$/,$$p' > "$@"
	astyle --project=none --suffix=none "$@"

doxygen: doxygen/c/$(HEADER) doxygen/cpp/$(HEADER)
	mkdir -p doc/c doc/cpp
	cd doxygen &&\
	doxygen ../doxygen_c.cfg >/dev/null &&\
	doxygen ../doxygen_cpp.cfg >/dev/null

clean_doxygen:
	$(RM) -rf doc doxygen

# Rules to generate clean comments files, to check spelling
#c_clean/$(HEADER):
# mkdir -p "$(@D)"
# printf '' | $(CC) -pipe -E -xc - -include $< -o /dev/stdout |\
# sed -n '/^$$/d;/^#/d;/^struct nsl_stats$$/,$$p' > "$@"
# astyle --project=none --suffix=none "$@"
# diff "doxygen/c/$(HEADER)" "$@" | grep '^<' |\
# sed 's/[/*<]/ /g;s/ \+$$//;s/^ \+//' > c.txt
#cpp_clean/$(HEADER):
# mkdir -p "$(@D)"
# printf '' | $(CXX) -pipe -E -xc++ - -include $< -o /dev/stdout |\
# sed -n '/^$$/d;/^#/d;/^class nsl_stats$$/,$$p' > "$@"
# astyle --project=none --suffix=none "$@"
# diff "doxygen/cpp/$(HEADER)" "$@" |\
# grep '^<' | sed 's/[/*<]/ /g;s/ \+$$//;s/^ \+//' > cpp.txt
#doxclean: doxygen c_clean/$(HEADER) cpp_clean/$(HEADER)

utest.o:
	printf 'int main(int argc,char**argv)%s'\
	 '{::testing::InitGoogleTest(&argc,argv);return RUN_ALL_TESTS();}' |\
	$(CXX) $(CPPFLAGS) -include gtest/gtest.h -DGTEST_HAS_PTHREAD=1\
	 -c -x c++ - -o "$@"

utest: utest.o utest_cpp.o utest_c.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS)\
	 -lgtest -lpthread -o "$@"

ifeq ($(DOCDIR),)
DOCDIR:=/usr/share/doc/$(PKG)-doc/
endif
ifeq ($(INSTALL),)
INSTALL:=install
endif
install:
	$(INSTALL) -m 644 -D $(HEADER) -t $(DESTDIR)/usr/include
	$(INSTALL) -d $(DESTDIR)$(DOCDIR)
	cp -a doc/* $(DESTDIR)$(DOCDIR)

deb:
	dpkg-buildpackage -b --no-sign

clean_deb:
	$(RM) -rf $(addprefix debian/,.debhelper debhelper-build-stamp\
	            files $(PKG)-*.substvars $(PKG)-dev $(PKG)-doc)

SRC_FILES:=$(HEADER) README.md Makefile LICENSE.txt $(wildcard doxygen_*.cfg)
SRC_NAME:=$(PKG)-$(ver_maj).$(ver_min)
TAR:=tar cfJ
LIB_SRC:=$(SRC_NAME).txz
$(LIB_SRC): $(SRC_FILES)
	$(TAR) "$@" $^ --transform "s!^!$(SRC_NAME)/!S"
srcpkg: $(LIB_SRC)

ifneq ($(call which,rpmbuild),)
rpm: $(LIB_SRC) rpm/SOURCES
	cp $(LIB_SRC) rpm/SOURCES/
	rpmbuild --define "debug_package %nil" --define "_topdir $(PWD)/rpm" \
	 -bb $(PKG).spec
endif # which rpmbuild
clean_rpm:
	$(RM) -rf rpm

ifneq ($(call which,makepkg),)
ARCHL_BLD:=archlinux/PKGBUILD
$(ARCHL_BLD): $(ARCHL_BLD).org | $(LIB_SRC)
	cp "$<" "$@"
	cp $(LIB_SRC) archlinux/
	printf "sha256sums=('%s')\n"\
	  $(firstword $(shell sha256sum $(LIB_SRC))) >> "$@"
pkg: $(ARCHL_BLD)
	cd archlinux && makepkg
endif # which makepkg
clean_pkg:
	$(RM) -rf $(addprefix archlinux/,PKGBUILD pkg src\
	            $(PKG)-*.txz $(PKG)-*.pkg.tar.zst)
