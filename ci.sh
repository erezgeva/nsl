#!/bin/bash -e
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
# CI (Continuous integration) script for containers
###############################################################################
distribution()
{
 if [[ -f /etc/debian_version ]]; then
   # Also Ubuntu and other Debian based distributions
   dist=debian
 else
   for n in /etc/*-release; do
     if [[ -f "$n" ]]; then
       case $n in
         /etc/os-release) ;;
         /etc/system-release) ;;
         *)
            m="${n%-release}"
            dist="${m#/etc/}"
            break
            ;;
       esac
     fi
   done
 fi
}
###############################################################################
eacmd()
{
 set +e
 out="`$@ 2>&1`"
 last_ret=$?
 set -e
}
eecmd()
{
 set +e
 out=$(eval "$@" 2>&1)
 last_ret=$?
 set -e
}
mquit()
{
 if [[ -n "$out" ]]; then
   echo "$out"
 fi
 echo "$color_red$color_red$@$color_norm"
 exit 1
}
###############################################################################
cmp_cmp()
{
 local -r baseOut='min 9223372036854775807, max -9223372036854775808
count 3, min 10, max 30, avg 20.000000, sq 466.666667, sd 10.000000
===============================================
start 10, end 60, step 10, size 5
bf: blow 0, above 0, value 0 0, value 10 0, value 20 0, value 30 0, value 40 0, value 50 0, value 60 0
 9: blow 1, above 0, value 0 1, value 10 0, value 20 0, value 30 0, value 40 0, value 50 0, value 60 0
10: blow 1, above 0, value 0 1, value 10 1, value 20 0, value 30 0, value 40 0, value 50 0, value 60 0
19: blow 1, above 0, value 0 1, value 10 2, value 20 0, value 30 0, value 40 0, value 50 0, value 60 0
20: blow 1, above 0, value 0 1, value 10 2, value 20 1, value 30 0, value 40 0, value 50 0, value 60 0
29: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 0, value 40 0, value 50 0, value 60 0
30: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 1, value 40 0, value 50 0, value 60 0
39: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 2, value 40 0, value 50 0, value 60 0
40: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 2, value 40 1, value 50 0, value 60 0
49: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 2, value 40 2, value 50 0, value 60 0
50: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 2, value 40 2, value 50 1, value 60 0
59: blow 1, above 0, value 0 1, value 10 2, value 20 2, value 30 2, value 40 2, value 50 2, value 60 0
60: blow 1, above 1, value 0 1, value 10 2, value 20 2, value 30 2, value 40 2, value 50 2, value 60 1
v2: blow 1, above 1, value 0 1, value 10 2, value 20 2, value 30 2, value 40 2, value 50 2, value 60 1'
 echo "====== compare compilations ======"
 local -i cnt
 for n in out/cmp_*; do
   if [[ -f "$n" ]] && [[ -x "$n" ]]; then
     cnt+=1
     curOut="$(eval "$n")"
     eecmd "diff <(printf \"$baseOut\") <(printf \"$curOut\")"
     if [[ -n "$out" ]]; then
       mquit "** $n fails"
     fi
   fi
 done
 [[ $cnt -gt 0 ]] || mquit "Fail to find anything to compare"
}
###############################################################################
main()
{
 local n m nocolor out dist curOut
 local -i last_ret jobs=1 # Number of Make parallel jobs
 if [[ "$GITHUB_ACTIONS" = "true" ]]; then
   nocolor=yes
 fi
 while getopts 'oj:' opt; do
   case $opt in
     o)
       nocolor=yes
       ;;
     j)
       jobs="0 + $OPTARG"
       ;;
   esac
 done
 if [[ $jobs -le 0 ]]; then
   jobs=1
 fi
 # Make sure we output to STDOUT directly, no pipes
 # check our teminal support coulors
 if [[ -z "$nocolor" ]] && [[ -t 1 ]] && tput setaf 1; then
   local -r esc=$(printf '\e[')
   local -r color_red=${esc}31m
   local -r color_blue=${esc}34m
   local -r color_norm=${esc}00m
   printf "$color_norm"
 fi
 eacmd git rev-parse --is-inside-work-tree
 if [[ $last_ret -eq 0 ]]; then
   local -r have_git=true
 else
   local -r have_git=false
 fi
 if $have_git && [[ -n "$(which reuse 2> /dev/null)" ]]; then
   # Ensure all files in project are properly copyright
   reuse lint
 fi
 case $(uname) in
   Linux)
     # Compile with all standards and the unit tests
     make -j$jobs
     # Run the unit tests
     ./utest
     # Compare all standards tests
     cmp_cmp
     # Get Linux distribution
     distribution
     # Build packages on specific distribution
     case $dist in
       debian)
         make -j$jobs deb
         ;;
       fedora|redhat)
         make -j$jobs rpm
         ;;
       arch)
         make -j$jobs pkg
         ;;
     esac
     ;;
   Darwin) # MacOS
     # Find latest GCC compiler
     # As cc and gcc are linked to clang
     local -i i=20
     until which gcc-$i > /dev/null;do i+=-1;done
     make -j$jobs CC=gcc-$i CXX=g++-$i all_gcc all_clang
     # Compare all standards tests
     cmp_cmp
     ;;
   MINGW64*)
     case $compiler in
       mingw64)
         # Compile with all standards
         make -j$jobs ST_MEAN_TYPE_DOUBLE=1 all_gcc
         # Compare all standards tests
         cmp_cmp
         ;;
       msvc)
         curl --retry 15 -s -L https://github.com/swig/cccl/raw/cccl-1.4/cccl -o /usr/bin/cccl
         chmod +x /usr/bin/cccl
         local -r EXT=.exe
         # Compile with all standards
         make -j$jobs CC=cccl CXX=cccl CC2=cccl ST_SKIP_MATH_LINK=1 EXT=$EXT all_gcc
         # Compare all standards tests
         cmp_cmp
         ;;
     esac
     ;;
 esac
}
main "$@"
